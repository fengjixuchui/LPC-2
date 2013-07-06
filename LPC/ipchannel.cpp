#include "ipchannel.h"
#include "base_type.pb.h"
#include "helpers.h"
#include "id_pool.h"

//! IOCP请求
struct Request
{
	OVERLAPPED ol;
	enum class OpCode {
		READ,	//!< 读取消息(有可能没读完一条)
		READ_REST,	//!< 读取消息剩余部分(有可能还有剩余)
		WRITE,	//!< 写入消息
		CONNECT, //!< 管道连接
		QUIT	//!< 退出线程
	} op;	//!< 操作的类型
	byte read_buff[InterProcessChannel::MAX_BUFF_SIZE];	//!< 读取缓冲区
	std::string write_buff;	//!< 写入缓冲区
	Request *prev;	//!< 存放消息前一部分的请求, 仅在 op == READ_REST 时有意义
	int num_prev_parts;	//!< 之前有多少个消息片段, 仅在 op == READ_REST 时有意义

	Request() {
		// 不清零会出现ERROR_INVALID_PARAMETER
		memset(&ol, 0, sizeof(ol));
	}
};

//! 用来管理所有Request的申请和回收
class RequestPool {
private:
	static CCriticalSection lock_;
	static std::vector<Request*> requsets_;

public:
	static Request *Get() {
		CSGuard l(&lock_);
		if (requsets_.empty())
			return new Request;
		auto ret = requsets_.back();
		requsets_.pop_back();
		return ret;
	}

	static void Put(Request *req) {
		CSGuard l(&lock_);
		if (req != NULL)
			requsets_.push_back(req);
	}
};

CCriticalSection RequestPool::lock_;

std::vector<Request*> RequestPool::requsets_;

const std::wstring InterProcessChannel::PIPE_PREFIX(L"\\\\.\\pipe\\");

CCriticalSection InterProcessChannel::singleton_lock_;

std::unordered_map<DWORD, InterProcessChannel*> InterProcessChannel::channels_;

/*!
所有构造和获取 InterProcessChannel 对象的工作都由此函数来完成, 以实现单例.
如果之前已经构造出来 InterProcessChannel 对象了, 直接根据其对应pid查询并返回; 
否则新构造一个对象并以pid为索引储存起来. 
通过CRITICAL_SECTION保证查询与构造的线程安全
\param [in] pid 想要与之通信的pid
\return pid对应的InterProcessChannel对象指针
*/
InterProcessChannel * InterProcessChannel::ToProcess( DWORD pid ) {
	CSGuard lock(&singleton_lock_);
	if (!channels_.count(pid)) {
		InterProcessChannel *ipc;
		try {
			ipc = new InterProcessChannel(pid);
		} catch(std::exception &e) {
			//
		}
		channels_.insert(std::make_pair(pid, ipc));
	}
	return channels_[pid];
}

/*!
\param [in] pid 想要与之通信的pid
\exception std::runtime_error 创建或连接管道时出现错误
*/
InterProcessChannel::InterProcessChannel( DWORD pid ) : to_pid_(pid) {
	DWORD max_pid = GetCurrentProcessId(), min_pid = pid;
	MaxMin(max_pid, min_pid);
	std::wstring pipe_name = PIPE_PREFIX + 
		I2WString(max_pid) + L"_" + I2WString(min_pid);
	DWORD err;
	connect_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);

	// 先尝试打开已有管道
	pipe_ = CreateFile(pipe_name.c_str(), GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (pipe_ == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		if (err != ERROR_FILE_NOT_FOUND)
			throw std::runtime_error("CreateFile failed. GetLastError == " + 
				I2String(err));

		// ERROR_NOT_FOUND 说明这个管道还没有创建, 所以需要创建一个
		pipe_ = CreateNamedPipe(pipe_name.c_str(), 
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			1, MAX_BUFF_SIZE, MAX_BUFF_SIZE, 0, NULL);
		if (pipe_ == INVALID_HANDLE_VALUE)
			throw std::runtime_error("CreateNamedPipe failed. GetLastError == " + 
				I2String(GetLastError()));

		// 等待对方连接
		Request *req = RequestPool::Get();
		req->ol.hEvent = connect_event_;
		req->op = Request::OpCode::CONNECT;
		BOOL conn_f = ConnectNamedPipe(pipe_, &req->ol);
		if (!conn_f) {
			err = GetLastError();
			if (err != ERROR_IO_PENDING && err != ERROR_PIPE_CONNECTED)
				throw std::runtime_error("ConnectNamedPipe failed. GetLastError == " + 
					I2String(err));
		}
	} else {
		// 管道已存在, 修改为消息模式
		DWORD mode = PIPE_READMODE_MESSAGE; 
		if (!SetNamedPipeHandleState(pipe_, &mode, NULL, NULL))
			throw std::runtime_error("SetNamedPipeHandleState failed. GetLastError == " + 
				I2String(GetLastError()));
		SetEvent(connect_event_);
	}

	// 为管道创建完成端口
	if ((completion_port_ = CreateIoCompletionPort(pipe_, NULL, 0, 0)) == NULL)
		throw std::runtime_error("CreateIoCompletionPort failed. GetLastError == " + 
			I2String(GetLastError()));

	// 添加结束响应
	AddHandler(kTerminate, [this] (IDType request_id, const std::string &serialized_param) {
		LpcMessage box;
		box.set_dispatch_id(request_id);
		Send(box.SerializeAsString());

		Request *req = RequestPool::Get();
		req->op = Request::OpCode::QUIT;
		PostQueuedCompletionStatus(completion_port_, 0, NULL, &req->ol);
	});

	// 等待连接建立
	if (WaitForSingleObject(connect_event_, INFINITE) != WAIT_OBJECT_0)
		throw std::runtime_error("WaitForSingleObject failed. GetLastError == " + 
			I2String(GetLastError()));
	
	// 第一次读取
	Request *req = RequestPool::Get();
	req->op = Request::OpCode::READ;
	if (ReadFile(pipe_, req->read_buff, MAX_BUFF_SIZE, NULL, &req->ol) == FALSE)
		if ((err = GetLastError()) != ERROR_IO_PENDING)
			throw std::runtime_error("ReadFile failed. GetLastError == " + 
				I2String(GetLastError()));

	// 没有异常, 创建线程开始处理IO结果
	CreateThread(NULL, 0, DispatchThread_, this, NULL, NULL);
	return ;
}

/*!
循环处理IO结果, 将获取到的序列化的消息通过 DispatchMessage_ 派发. 
获取到OpCode为QUIT的结果时退出循环, 结束线程.
\param [in] pp 创建这个线程的 InterProcessChannel 对象的指针
\sa DispatchMessage_
*/
DWORD CALLBACK InterProcessChannel::DispatchThread_( LPVOID pp ) {
	InterProcessChannel *self = static_cast<InterProcessChannel*>(pp);
	BOOL err;
	DWORD bytes_transferred;
	ULONG completion_key;
	Request *req;

	do {
		err = GetQueuedCompletionStatus(self->completion_port_, &bytes_transferred,
			&completion_key, reinterpret_cast<LPOVERLAPPED*>(&req), INFINITE);
		if (err == FALSE) {
			err = GetLastError();
			if (err != ERROR_MORE_DATA) {
				continue;
			}
		}

		switch (req->op) {
		case Request::OpCode::READ: {
			// 如果消息还没读完, 继续读剩余部分
			if (err == ERROR_MORE_DATA) {
				Request *req_rest = RequestPool::Get();
				req_rest->op = Request::OpCode::READ_REST;
				req_rest->prev = req;
				req_rest->num_prev_parts = 1;
				ReadFile(self->pipe_, req_rest->read_buff, 
					MAX_BUFF_SIZE, NULL, &req_rest->ol);
				break;
			}

			// 读取了一个完整的消息, 派发给对应处理回调
			DispatchMessage_(self, req->read_buff, bytes_transferred);

			// 读下一条消息
			ReadFile(self->pipe_, req->read_buff, MAX_BUFF_SIZE, NULL, &req->ol);
		}
		break;
		case Request::OpCode::READ_REST: {
			// 如果消息还没读完, 继续读剩余部分
			if (err == ERROR_MORE_DATA) {
				Request *req_rest = RequestPool::Get();
				req_rest->op = Request::OpCode::READ_REST;
				req_rest->prev = req;
				req_rest->num_prev_parts = req->num_prev_parts + 1;
				ReadFile(self->pipe_, req_rest->read_buff, 
					MAX_BUFF_SIZE, NULL, &req_rest->ol);
				break;
			}

			// 读完了消息的剩余部分, 拼接成一个完整的消息
			UINT size = req->num_prev_parts * MAX_BUFF_SIZE + bytes_transferred;
			byte *buf = new byte[size];
			memcpy(buf + req->num_prev_parts * MAX_BUFF_SIZE, req->read_buff, bytes_transferred);
			Request *cur = req->prev;
			for (int i = cur->num_prev_parts; i >= 0; i--) {
				memcpy(buf + i * MAX_BUFF_SIZE, cur->read_buff, MAX_BUFF_SIZE);
				Request *t = cur; 
				cur = cur->prev;
				RequestPool::Put(t);
			}
			DispatchMessage_(self, buf, size);

			// 读下一条消息
			req->op = Request::OpCode::READ;
			ReadFile(self->pipe_, req->read_buff, MAX_BUFF_SIZE, NULL, &req->ol);
		}
		break;
		case Request::OpCode::WRITE: {
			req->write_buff.swap(std::string());
			RequestPool::Put(req);
		}
		break;
		}
	} while (req->op != Request::OpCode::QUIT);
	RequestPool::Put(req);

	channels_.erase(self->to_pid_);
	delete self;

	return 0;
}

InterProcessChannel::~InterProcessChannel() {
	CloseHandle(pipe_);
	CloseHandle(connect_event_);
	CloseHandle(completion_port_);
}

/*!
先将字符数组解析成容器消息, 从中取得方法id, 然后将编号和序列化过的参数传递个对应处理函数
\param [in] self 要派发消息的 InterProcessChannel 对象指针
\param [in] serialized_message 序列化的容器消息, 其中包含方法id, 调用编号和序列化的参数
\param [in] size serialized_message的长度
\return 是否有对应的处理函数
*/
bool InterProcessChannel::DispatchMessage_( InterProcessChannel *self, 
										   byte *serialized_message, UINT size ) {
	LpcMessage msg;
	msg.ParseFromArray(serialized_message, size);
	ReadLockGuard rl(&self->handlers_lock_);
	if (self->handlers_.count(msg.dispatch_id()) == 0)
		return false;
	self->handlers_[msg.dispatch_id()](msg.request_id(), msg.content());
	return true;
}

/*!
同时还会注册占用dispatch_id
*/
void InterProcessChannel::AddHandler( IDType dispatch_id, HandlerType handler ) {
	ReadLockGuard wl(&handlers_lock_);
	id_pool_.Occupy(dispatch_id);
	handlers_.insert(std::make_pair(dispatch_id, handler));
}

/*!
同时还会注册占用dispatch_id
*/
void InterProcessChannel::AddHandlers(const std::vector<std::pair<IDType, HandlerType>> &handlers) {
	ReadLockGuard wl(&handlers_lock_);
	for (auto &pair : handlers) {
		id_pool_.Occupy(pair.first);
		handlers_.insert(pair);
	}
}

/*!
不会自动注册占用dispatch_id但会在完成时自动取消占用
*/
void InterProcessChannel::AddOneTimeHandler( IDType dispatch_id, HandlerType handler ) {
	ReadLockGuard wl(&handlers_lock_);
	handlers_.insert(std::make_pair(dispatch_id, [this, dispatch_id, handler] (
		IDType request_id, const std::string &serialized_param) {
		handler(request_id, serialized_param);
		RemoveHandler(dispatch_id);
	}));
}

/*!
同时取消dispatch_id的占用
*/
void InterProcessChannel::RemoveHandler( IDType dispatch_id ) {
	ReadLockGuard wl(&handlers_lock_);
	handlers_.erase(dispatch_id);
	id_pool_.Put(dispatch_id);
}

/*!
同时取消dispatch_id的占用
*/
void InterProcessChannel::RemoveHandlers( std::vector<IDType> dispatch_ids ) {
	ReadLockGuard wl(&handlers_lock_);
	for (auto dispatch_id : dispatch_ids) {
		handlers_.erase(dispatch_id);
		id_pool_.Put(dispatch_id);
	}
}

InterProcessChannel::HandlerType && InterProcessChannel::MoveHandler(IDType dispatch_id) {
	ReadLockGuard wl(&handlers_lock_);
	return std::move(handlers_[dispatch_id]);
}

/*!
发送lpc消息, 消息应该是由LpcMessage序列化而来的.
\param [in] serialized_message 序列化好的消息. 类型是右值引用, 所以非右值的话要std::move一下
\return 是否发送成功
*/
bool InterProcessChannel::Send( const std::string &serialized_message ) const {
	Request *req = RequestPool::Get();
	req->op = Request::OpCode::WRITE;
	req->write_buff = serialized_message;
	if (WriteFile(pipe_, req->write_buff.c_str(), 
			req->write_buff.size(), NULL, &req->ol) == FALSE) {
		DWORD err = GetLastError();
		if (err != ERROR_IO_PENDING) {
			return false;
		}
	}
	return true;
}

/*!
这个函数是同步的, 发送出结束消息后还要等待对方的回复
*/
bool InterProcessChannel::Close() {
	LpcMessage box;
	box.set_dispatch_id(kTerminate);
	int request_id = GetIdleID();
	box.set_request_id(request_id);

	HANDLE recv = CreateEvent(NULL, TRUE, FALSE, NULL);
	AddOneTimeHandler(request_id, [recv] (IDType request_id, const std::string &serialized_param) {
		SetEvent(recv);
	});
	if (!Send(box.SerializeAsString()))
		return false;

	WaitForSingleObject(recv, INFINITE);
	CloseHandle(recv);
	return true;
}

/*!
一定要在所有服务都添加好之后再调用此函数, 
否则可能会导致一个本该是某个服务函数的ID被当做临时ID分配出去
*/
int InterProcessChannel::GetIdleID() {
	return id_pool_.Get();
}

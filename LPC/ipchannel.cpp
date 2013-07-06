#include "ipchannel.h"
#include "base_type.pb.h"
#include "helpers.h"
#include "id_pool.h"

//! IOCP����
struct Request
{
	OVERLAPPED ol;
	enum class OpCode {
		READ,	//!< ��ȡ��Ϣ(�п���û����һ��)
		READ_REST,	//!< ��ȡ��Ϣʣ�ಿ��(�п��ܻ���ʣ��)
		WRITE,	//!< д����Ϣ
		CONNECT, //!< �ܵ�����
		QUIT	//!< �˳��߳�
	} op;	//!< ����������
	byte read_buff[InterProcessChannel::MAX_BUFF_SIZE];	//!< ��ȡ������
	std::string write_buff;	//!< д�뻺����
	Request *prev;	//!< �����Ϣǰһ���ֵ�����, ���� op == READ_REST ʱ������
	int num_prev_parts;	//!< ֮ǰ�ж��ٸ���ϢƬ��, ���� op == READ_REST ʱ������

	Request() {
		// ����������ERROR_INVALID_PARAMETER
		memset(&ol, 0, sizeof(ol));
	}
};

//! ������������Request������ͻ���
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
���й���ͻ�ȡ InterProcessChannel ����Ĺ������ɴ˺��������, ��ʵ�ֵ���.
���֮ǰ�Ѿ�������� InterProcessChannel ������, ֱ�Ӹ������Ӧpid��ѯ������; 
�����¹���һ��������pidΪ������������. 
ͨ��CRITICAL_SECTION��֤��ѯ�빹����̰߳�ȫ
\param [in] pid ��Ҫ��֮ͨ�ŵ�pid
\return pid��Ӧ��InterProcessChannel����ָ��
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
\param [in] pid ��Ҫ��֮ͨ�ŵ�pid
\exception std::runtime_error ���������ӹܵ�ʱ���ִ���
*/
InterProcessChannel::InterProcessChannel( DWORD pid ) : to_pid_(pid) {
	DWORD max_pid = GetCurrentProcessId(), min_pid = pid;
	MaxMin(max_pid, min_pid);
	std::wstring pipe_name = PIPE_PREFIX + 
		I2WString(max_pid) + L"_" + I2WString(min_pid);
	DWORD err;
	connect_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);

	// �ȳ��Դ����йܵ�
	pipe_ = CreateFile(pipe_name.c_str(), GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (pipe_ == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		if (err != ERROR_FILE_NOT_FOUND)
			throw std::runtime_error("CreateFile failed. GetLastError == " + 
				I2String(err));

		// ERROR_NOT_FOUND ˵������ܵ���û�д���, ������Ҫ����һ��
		pipe_ = CreateNamedPipe(pipe_name.c_str(), 
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			1, MAX_BUFF_SIZE, MAX_BUFF_SIZE, 0, NULL);
		if (pipe_ == INVALID_HANDLE_VALUE)
			throw std::runtime_error("CreateNamedPipe failed. GetLastError == " + 
				I2String(GetLastError()));

		// �ȴ��Է�����
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
		// �ܵ��Ѵ���, �޸�Ϊ��Ϣģʽ
		DWORD mode = PIPE_READMODE_MESSAGE; 
		if (!SetNamedPipeHandleState(pipe_, &mode, NULL, NULL))
			throw std::runtime_error("SetNamedPipeHandleState failed. GetLastError == " + 
				I2String(GetLastError()));
		SetEvent(connect_event_);
	}

	// Ϊ�ܵ�������ɶ˿�
	if ((completion_port_ = CreateIoCompletionPort(pipe_, NULL, 0, 0)) == NULL)
		throw std::runtime_error("CreateIoCompletionPort failed. GetLastError == " + 
			I2String(GetLastError()));

	// ��ӽ�����Ӧ
	AddHandler(kTerminate, [this] (IDType request_id, const std::string &serialized_param) {
		LpcMessage box;
		box.set_dispatch_id(request_id);
		Send(box.SerializeAsString());

		Request *req = RequestPool::Get();
		req->op = Request::OpCode::QUIT;
		PostQueuedCompletionStatus(completion_port_, 0, NULL, &req->ol);
	});

	// �ȴ����ӽ���
	if (WaitForSingleObject(connect_event_, INFINITE) != WAIT_OBJECT_0)
		throw std::runtime_error("WaitForSingleObject failed. GetLastError == " + 
			I2String(GetLastError()));
	
	// ��һ�ζ�ȡ
	Request *req = RequestPool::Get();
	req->op = Request::OpCode::READ;
	if (ReadFile(pipe_, req->read_buff, MAX_BUFF_SIZE, NULL, &req->ol) == FALSE)
		if ((err = GetLastError()) != ERROR_IO_PENDING)
			throw std::runtime_error("ReadFile failed. GetLastError == " + 
				I2String(GetLastError()));

	// û���쳣, �����߳̿�ʼ����IO���
	CreateThread(NULL, 0, DispatchThread_, this, NULL, NULL);
	return ;
}

/*!
ѭ������IO���, ����ȡ�������л�����Ϣͨ�� DispatchMessage_ �ɷ�. 
��ȡ��OpCodeΪQUIT�Ľ��ʱ�˳�ѭ��, �����߳�.
\param [in] pp ��������̵߳� InterProcessChannel �����ָ��
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
			// �����Ϣ��û����, ������ʣ�ಿ��
			if (err == ERROR_MORE_DATA) {
				Request *req_rest = RequestPool::Get();
				req_rest->op = Request::OpCode::READ_REST;
				req_rest->prev = req;
				req_rest->num_prev_parts = 1;
				ReadFile(self->pipe_, req_rest->read_buff, 
					MAX_BUFF_SIZE, NULL, &req_rest->ol);
				break;
			}

			// ��ȡ��һ����������Ϣ, �ɷ�����Ӧ����ص�
			DispatchMessage_(self, req->read_buff, bytes_transferred);

			// ����һ����Ϣ
			ReadFile(self->pipe_, req->read_buff, MAX_BUFF_SIZE, NULL, &req->ol);
		}
		break;
		case Request::OpCode::READ_REST: {
			// �����Ϣ��û����, ������ʣ�ಿ��
			if (err == ERROR_MORE_DATA) {
				Request *req_rest = RequestPool::Get();
				req_rest->op = Request::OpCode::READ_REST;
				req_rest->prev = req;
				req_rest->num_prev_parts = req->num_prev_parts + 1;
				ReadFile(self->pipe_, req_rest->read_buff, 
					MAX_BUFF_SIZE, NULL, &req_rest->ol);
				break;
			}

			// ��������Ϣ��ʣ�ಿ��, ƴ�ӳ�һ����������Ϣ
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

			// ����һ����Ϣ
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
�Ƚ��ַ����������������Ϣ, ����ȡ�÷���id, Ȼ�󽫱�ź����л����Ĳ������ݸ���Ӧ������
\param [in] self Ҫ�ɷ���Ϣ�� InterProcessChannel ����ָ��
\param [in] serialized_message ���л���������Ϣ, ���а�������id, ���ñ�ź����л��Ĳ���
\param [in] size serialized_message�ĳ���
\return �Ƿ��ж�Ӧ�Ĵ�����
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
ͬʱ����ע��ռ��dispatch_id
*/
void InterProcessChannel::AddHandler( IDType dispatch_id, HandlerType handler ) {
	ReadLockGuard wl(&handlers_lock_);
	id_pool_.Occupy(dispatch_id);
	handlers_.insert(std::make_pair(dispatch_id, handler));
}

/*!
ͬʱ����ע��ռ��dispatch_id
*/
void InterProcessChannel::AddHandlers(const std::vector<std::pair<IDType, HandlerType>> &handlers) {
	ReadLockGuard wl(&handlers_lock_);
	for (auto &pair : handlers) {
		id_pool_.Occupy(pair.first);
		handlers_.insert(pair);
	}
}

/*!
�����Զ�ע��ռ��dispatch_id���������ʱ�Զ�ȡ��ռ��
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
ͬʱȡ��dispatch_id��ռ��
*/
void InterProcessChannel::RemoveHandler( IDType dispatch_id ) {
	ReadLockGuard wl(&handlers_lock_);
	handlers_.erase(dispatch_id);
	id_pool_.Put(dispatch_id);
}

/*!
ͬʱȡ��dispatch_id��ռ��
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
����lpc��Ϣ, ��ϢӦ������LpcMessage���л�������.
\param [in] serialized_message ���л��õ���Ϣ. ��������ֵ����, ���Է���ֵ�Ļ�Ҫstd::moveһ��
\return �Ƿ��ͳɹ�
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
���������ͬ����, ���ͳ�������Ϣ��Ҫ�ȴ��Է��Ļظ�
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
һ��Ҫ�����з�����Ӻ�֮���ٵ��ô˺���, 
������ܻᵼ��һ��������ĳ����������ID��������ʱID�����ȥ
*/
int InterProcessChannel::GetIdleID() {
	return id_pool_.Get();
}

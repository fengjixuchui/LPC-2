#pragma once
#include "windows.h"
#undef ERROR
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include "critical_section.h"
#include "rwlock.h"
#include "id_pool.h"

/*!
\brief 跨进程通信调用

使用命名管道实现了与目标进程间的双向通信, 接收到的消息通过注册handler进行处理.
此类不能自己构造, 必须通过 ToProcess 获取对象指针, 保证单例.
*/
class InterProcessChannel {
public:
	typedef IDPool::IDType IDType;
	typedef std::function<void(IDType request_id, 
		const std::string &serialized_param)> HandlerType;
	
	//! 获取与指定进程进行通信的 InterProcessChannel 对象指针
	static InterProcessChannel *ToProcess(DWORD pid);

	//! 添加一个处理指定消息的处理函数
	void AddHandler(IDType dispatch_id, HandlerType handler);
	//! 添加一组处理指定消息的处理函数
	void AddHandlers(const std::vector<std::pair<IDType, HandlerType>> &handlers);
	//! 添加一个仅响应一次的处理函数
	void AddOneTimeHandler(IDType dispatch_id, HandlerType handler);
	//! 移除一个处理指定消息的处理函数
	void RemoveHandler(IDType dispatch_id);
	//! 移除一组处理指定消息的处理函数
	void RemoveHandlers(std::vector<IDType> dispatch_ids);
	//! 移动一个处理指定消息的处理函数
	HandlerType && MoveHandler(IDType dispatch_id);
	//! 获取一个空闲的ID
	int GetIdleID();
	//! 发送序列化好的message
	bool Send(const std::string &serialized_message) const;
	//! 结束通信
	bool Close();
private:
	//! 构造与指定pid通信的对象
	InterProcessChannel(DWORD pid);
	InterProcessChannel(const InterProcessChannel&);
	~InterProcessChannel();
	InterProcessChannel & operator =(const InterProcessChannel&);
	//! IO结果处理线程函数, 在这个线程里将消息分发给对应的处理函数
	static DWORD CALLBACK DispatchThread_(LPVOID pp);
	//! 派发消息
	static bool DispatchMessage_(InterProcessChannel *self, 
		byte *serialized_message, UINT size);

public:
	//! 读取缓冲区大小
	static const int MAX_BUFF_SIZE = 4 * 1024;

private:
	//! 管道名称的前缀 \\.\pipe\ 
	static const std::wstring PIPE_PREFIX;
	//! 在 ToProcess 中保证线程安全的锁
	static CCriticalSection singleton_lock_;
	//! 所有已构造出来的对象, key=pid
	static std::unordered_map<DWORD, InterProcessChannel*> channels_;

	//! 结束消息
	static const int kTerminate = 0;

	DWORD to_pid_;	//!< 进行通信的进程pid
	HANDLE pipe_;	//!< 管道的句柄
	HANDLE completion_port_;	//!< 完成端口句柄
	HANDLE connect_event_;	//!< 连接成功时被设置
	std::unordered_map<IDType, HandlerType> handlers_;	//!< 派发id和处理函数的映射
	IDPool id_pool_;	//!< id管理器
	RWLock handlers_lock_;	//!< 保护handlers_的读写锁
};

//! 向调用进程发送返回值
template<typename T> class InterProcessReturn {
public:
	/*!
	\brief 构造一个InterProcessReturn对象
	\param [in] ipc 发来调用请求的 InterProcessChannel 对象指针
	\param [in] dispatch_id 调用请求包含的request_id
	*/
	InterProcessReturn(InterProcessChannel *ipc, int dispatch_id) : 
		ipc_(ipc), dispatch_id_(dispatch_id){}
	/*!
	\brief 向调用进程返回结果
	\param [in] return_val 返回值
	\return 返回发送是否成功
	*/
	bool Return(const T &return_val) const {
		LpcMessage box;
		box.set_dispatch_id(dispatch_id_);
		box.set_content(return_val.SerializeAsString());
		return ipc_->Send(box.SerializeAsString());
	}

private:
	InterProcessChannel *ipc_;
	int dispatch_id_;
};

//! 向调用进程推送结果, 比 InterProcessReturn 多携带一个停止函数的id信息
template<typename T> class InterProcessPush {
public:
	/*!
	\brief 构造一个InterProcessPush对象
	\param [in] ipc 发来调用请求的 InterProcessChannel 对象指针
	\param [in] dispatch_id 调用请求包含的request_id
	\param [in] request_id 停止推送函数的id或空
	*/
	InterProcessPush(InterProcessChannel *ipc, int dispatch_id, int request_id) : 
		ipc_(ipc), dispatch_id_(dispatch_id), request_id_(request_id) {}
	/*!
	\brief 向调用进程推送结果
	\param [in] return_val 要推送的结果
	\return 发送是否成功
	*/
	bool Push(const T &return_val) const {
		LpcMessage box;
		box.set_dispatch_id(dispatch_id_);
		box.set_request_id(request_id_);
		box.set_content(return_val.SerializeAsString());
		return ipc_->Send(box.SerializeAsString());
	}

private:
	InterProcessChannel *ipc_;
	int dispatch_id_, request_id_;
};

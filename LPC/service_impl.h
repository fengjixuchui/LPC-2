#pragma once
#include "ipchannel.h"
#include <functional>
#include <unordered_map>

//! 提供了跨进程调用中service的通用实现
class ServiceImpl {
public:
	typedef InterProcessChannel::IDType IDType;
	typedef InterProcessChannel::HandlerType HandlerType;

	ServiceImpl(InterProcessChannel *ipc) : ipc_(ipc) {}

	//! 注册一个普通函数
	template<typename P, typename R> 
	void RegisterService(IDType method_id, 
		std::function<void(const P&, const InterProcessReturn<R>&)> service);
	//! 注册一个推送函数
	template<typename P, typename R> 
	void RegisterPushService(IDType method_id, 
		std::function<void(const P&, void *&, const InterProcessPush<R>&)> push_service, 
		std::function<void(void *)> stop_service,
		std::unordered_map<IDType, void*> &stop_signals);
	void RemoveService(IDType method_id);

private:
	InterProcessChannel *ipc_;
};

#include "service_impl.cpp"
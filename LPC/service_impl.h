#pragma once
#include "ipchannel.h"
#include <functional>
#include <unordered_map>

//! �ṩ�˿���̵�����service��ͨ��ʵ��
class ServiceImpl {
public:
	typedef InterProcessChannel::IDType IDType;
	typedef InterProcessChannel::HandlerType HandlerType;

	ServiceImpl(InterProcessChannel *ipc) : ipc_(ipc) {}

	//! ע��һ����ͨ����
	template<typename P, typename R> 
	void RegisterService(IDType method_id, 
		std::function<void(const P&, const InterProcessReturn<R>&)> service);
	//! ע��һ�����ͺ���
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
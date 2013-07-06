#pragma once

#include "ipchannel.h"
#include <functional>

//! �ṩ�˿���̵�����stub��ͨ��ʵ��
class StubImpl {
public:
	typedef InterProcessChannel::IDType IDType;

	StubImpl(InterProcessChannel *ipc) : ipc_(ipc) {}

	//! ���ͺ������õ�ͬ���汾
	template<typename P, typename R> R CallPushSync(IDType method_id, const P &params);
	//! ���ͺ������õ��첽�汾
	template<typename P, typename R> 
	void CallPushAsync(IDType method_id, const P &params, std::function<bool(const R&)> cb);

	//! ��ͨ�������õ�ͬ���汾
	template<typename P, typename R> R CallSync(IDType method_id, const P &params);
	//! ��ͨ�������õ��첽�汾
	template<typename P, typename R> 
	void CallAsync(IDType method_id, const P &params, std::function<void(const R&)> cb);

private:
	//! ����ֹͣ���ͺ���
	void StopPush_(int stop_handler_id, int return_id);

private:
	InterProcessChannel *ipc_;
};

#include "stub_impl.cpp"

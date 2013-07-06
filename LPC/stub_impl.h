#pragma once

#include "ipchannel.h"
#include <functional>

//! 提供了跨进程调用中stub的通用实现
class StubImpl {
public:
	typedef InterProcessChannel::IDType IDType;

	StubImpl(InterProcessChannel *ipc) : ipc_(ipc) {}

	//! 推送函数调用的同步版本
	template<typename P, typename R> R CallPushSync(IDType method_id, const P &params);
	//! 推送函数调用的异步版本
	template<typename P, typename R> 
	void CallPushAsync(IDType method_id, const P &params, std::function<bool(const R&)> cb);

	//! 普通函数调用的同步版本
	template<typename P, typename R> R CallSync(IDType method_id, const P &params);
	//! 普通函数调用的异步版本
	template<typename P, typename R> 
	void CallAsync(IDType method_id, const P &params, std::function<void(const R&)> cb);

private:
	//! 调用停止推送函数
	void StopPush_(int stop_handler_id, int return_id);

private:
	InterProcessChannel *ipc_;
};

#include "stub_impl.cpp"

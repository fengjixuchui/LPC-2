#include "base_type.pb.h"

template<typename P, typename R> 
R StubImpl::CallPushSync( IDType method_id, const P &params ) {
	int return_id = ipc_->GetIdleID();

	LpcMessage box;
	box.set_dispatch_id(method_id);
	box.set_request_id(return_id);
	box.set_content(params.SerializeAsString());

	HANDLE finish_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	R return_val;
	ipc_->AddOneTimeHandler(return_id, [this, return_id, finish_event, &return_val] 
		(int stop_handler_id, const std::string &serialized_return_val) {
			return_val.ParseFromString(serialized_return_val);
			SetEvent(finish_event);
			StopPush_(stop_handler_id, return_id);
	});
	
	ipc_->Send(box.SerializeAsString());
	WaitForSingleObject(finish_event, INFINITE);
	CloseHandle(finish_event);
	return return_val;
}


template<typename P, typename R>
void StubImpl::CallPushAsync( IDType method_id, const P &params, std::function<bool(const R&)> cb ) {
	int return_id = ipc_->GetIdleID();

	LpcMessage box;
	box.set_dispatch_id(method_id);
	box.set_request_id(return_id);
	box.set_content(params.SerializeAsString());

	ipc_->AddHandler(return_id, [this, cb, return_id] 
		(int stop_handler_id, const std::string &serialized_return_val) {
			R return_val;
			return_val.ParseFromString(serialized_return_val);
			if (!cb(return_val)) {
				std::function<void(int, const std::string&)> self = ipc_->MoveHandler(return_id);
				ipc_->RemoveHandler(return_id);
				StopPush_(stop_handler_id, return_id);
			}
	});

	ipc_->Send(box.SerializeAsString());
}

template<typename P, typename R>
R StubImpl::CallSync( IDType method_id, const P &params ) {
	int return_id = ipc_->GetIdleID();

	LpcMessage box;
	box.set_dispatch_id(method_id);
	box.set_request_id(return_id);
	box.set_content(params.SerializeAsString());

	HANDLE finish_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	R return_val;
	ipc_->AddOneTimeHandler(return_id, [&return_val, finish_event] 
		(int, const std::string &serialized_return_val) {
			return_val.ParseFromString(serialized_return_val);
			SetEvent(finish_event);
	});

	ipc_->Send(box.SerializeAsString());
	WaitForSingleObject(finish_event, INFINITE);
	SetEvent(finish_event);
	return return_val;
}

template<typename P, typename R>
void StubImpl::CallAsync( IDType method_id, const P &params, std::function<void(const R&)> cb ) {
	int return_id = ipc_->GetIdleID();

	LpcMessage box;
	box.set_dispatch_id(method_id);
	box.set_request_id(return_id);
	box.set_content(params.SerializeAsString());

	ipc_->AddOneTimeHandler(return_id, [cb] 
		(int, const std::string &serialized_return_val) {
			R return_val;
			return_val.ParseFromString(serialized_return_val);
			cb(return_val);
	});

	ipc_->Send(box.SerializeAsString());
}

inline void StubImpl::StopPush_( int stop_handler_id, int return_id ) {
	LpcMessage box;
	box.set_dispatch_id(stop_handler_id);
	box.set_request_id(return_id);
	ipc_->Send(box.SerializeAsString());
}

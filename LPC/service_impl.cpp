template<typename P, typename R> 
void ServiceImpl::RegisterService(IDType method_id, 
					std::function<void(const P&, const InterProcessReturn<R>&)> service) {
	ipc_->AddHandler(method_id, [this, service] 
		(IDType return_id, const std::string &serialized_params) {
			P params;
			params.ParseFromString(serialized_params);
			service(params, InterProcessReturn<R>(ipc_, return_id));
	});
}

template<typename P, typename R> 
void ServiceImpl::RegisterPushService(IDType method_id, 
						 std::function<void(const P&, void *&, const InterProcessPush<R>&)> push_service, 
						 std::function<void(void*)> stop_service,
						 std::unordered_map<IDType, void*> &stop_signals) {
	ipc_->AddHandler(method_id, [this, push_service, stop_service, &stop_signals]
		(IDType return_id, const std::string &serialized_params) {
			IDType stop_id = ipc_->GetIdleID();

			// 添加停止推送函数
			ipc_->AddOneTimeHandler(stop_id, [this, stop_service, &stop_signals] 
				(IDType return_id, const std::string &serialized_params) {
					stop_service(stop_signals.at(return_id));
					stop_signals.erase(return_id);
			});

			// 调用推送函数
			P params;
			params.ParseFromString(serialized_params);
			push_service(params, stop_signals[return_id], 
				InterProcessPush<R>(ipc_, return_id, stop_id));
	});
}

inline void ServiceImpl::RemoveService( IDType method_id ) {
	ipc_->RemoveHandler(method_id);
}

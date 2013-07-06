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
\brief �����ͨ�ŵ���

ʹ�������ܵ�ʵ������Ŀ����̼��˫��ͨ��, ���յ�����Ϣͨ��ע��handler���д���.
���಻���Լ�����, ����ͨ�� ToProcess ��ȡ����ָ��, ��֤����.
*/
class InterProcessChannel {
public:
	typedef IDPool::IDType IDType;
	typedef std::function<void(IDType request_id, 
		const std::string &serialized_param)> HandlerType;
	
	//! ��ȡ��ָ�����̽���ͨ�ŵ� InterProcessChannel ����ָ��
	static InterProcessChannel *ToProcess(DWORD pid);

	//! ���һ������ָ����Ϣ�Ĵ�����
	void AddHandler(IDType dispatch_id, HandlerType handler);
	//! ���һ�鴦��ָ����Ϣ�Ĵ�����
	void AddHandlers(const std::vector<std::pair<IDType, HandlerType>> &handlers);
	//! ���һ������Ӧһ�εĴ�����
	void AddOneTimeHandler(IDType dispatch_id, HandlerType handler);
	//! �Ƴ�һ������ָ����Ϣ�Ĵ�����
	void RemoveHandler(IDType dispatch_id);
	//! �Ƴ�һ�鴦��ָ����Ϣ�Ĵ�����
	void RemoveHandlers(std::vector<IDType> dispatch_ids);
	//! �ƶ�һ������ָ����Ϣ�Ĵ�����
	HandlerType && MoveHandler(IDType dispatch_id);
	//! ��ȡһ�����е�ID
	int GetIdleID();
	//! �������л��õ�message
	bool Send(const std::string &serialized_message) const;
	//! ����ͨ��
	bool Close();
private:
	//! ������ָ��pidͨ�ŵĶ���
	InterProcessChannel(DWORD pid);
	InterProcessChannel(const InterProcessChannel&);
	~InterProcessChannel();
	InterProcessChannel & operator =(const InterProcessChannel&);
	//! IO��������̺߳���, ������߳��ｫ��Ϣ�ַ�����Ӧ�Ĵ�����
	static DWORD CALLBACK DispatchThread_(LPVOID pp);
	//! �ɷ���Ϣ
	static bool DispatchMessage_(InterProcessChannel *self, 
		byte *serialized_message, UINT size);

public:
	//! ��ȡ��������С
	static const int MAX_BUFF_SIZE = 4 * 1024;

private:
	//! �ܵ����Ƶ�ǰ׺ \\.\pipe\ 
	static const std::wstring PIPE_PREFIX;
	//! �� ToProcess �б�֤�̰߳�ȫ����
	static CCriticalSection singleton_lock_;
	//! �����ѹ�������Ķ���, key=pid
	static std::unordered_map<DWORD, InterProcessChannel*> channels_;

	//! ������Ϣ
	static const int kTerminate = 0;

	DWORD to_pid_;	//!< ����ͨ�ŵĽ���pid
	HANDLE pipe_;	//!< �ܵ��ľ��
	HANDLE completion_port_;	//!< ��ɶ˿ھ��
	HANDLE connect_event_;	//!< ���ӳɹ�ʱ������
	std::unordered_map<IDType, HandlerType> handlers_;	//!< �ɷ�id�ʹ�������ӳ��
	IDPool id_pool_;	//!< id������
	RWLock handlers_lock_;	//!< ����handlers_�Ķ�д��
};

//! ����ý��̷��ͷ���ֵ
template<typename T> class InterProcessReturn {
public:
	/*!
	\brief ����һ��InterProcessReturn����
	\param [in] ipc ������������� InterProcessChannel ����ָ��
	\param [in] dispatch_id �������������request_id
	*/
	InterProcessReturn(InterProcessChannel *ipc, int dispatch_id) : 
		ipc_(ipc), dispatch_id_(dispatch_id){}
	/*!
	\brief ����ý��̷��ؽ��
	\param [in] return_val ����ֵ
	\return ���ط����Ƿ�ɹ�
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

//! ����ý������ͽ��, �� InterProcessReturn ��Я��һ��ֹͣ������id��Ϣ
template<typename T> class InterProcessPush {
public:
	/*!
	\brief ����һ��InterProcessPush����
	\param [in] ipc ������������� InterProcessChannel ����ָ��
	\param [in] dispatch_id �������������request_id
	\param [in] request_id ֹͣ���ͺ�����id���
	*/
	InterProcessPush(InterProcessChannel *ipc, int dispatch_id, int request_id) : 
		ipc_(ipc), dispatch_id_(dispatch_id), request_id_(request_id) {}
	/*!
	\brief ����ý������ͽ��
	\param [in] return_val Ҫ���͵Ľ��
	\return �����Ƿ�ɹ�
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

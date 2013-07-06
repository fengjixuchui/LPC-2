#include "rwlock.h"
#include "helpers.h"
RWLock::RWLock() {
	reader_count_ = 0;
	no_readers_ = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (no_readers_ == NULL)
		throw std::runtime_error("RWLock��CreateEvent����, ������ = " + 
		I2String(GetLastError()));
}

RWLock::~RWLock() {
	WaitForSingleObject(no_readers_, INFINITE);
	CloseHandle(no_readers_);
}

void RWLock::ReaderEnter() {
	// �赲д��
	CSGuard w(&write_lock_);
	// ��֤״̬ת����ԭ����
	CSGuard r(&read_lock_);
	// ���֮ǰû�ж���, ��no_readers_������״̬, ��Ҫ����ʹд�ߵȴ�
	if (++reader_count_ == 1)
		ResetEvent(no_readers_);
}

void RWLock::ReaderLeave() {
	// ��֤״̬ת����ԭ����
	CSGuard r(&read_lock_);
	// ���û�ж�����, ����no_readers_ʹд��д��
	if (--reader_count_ == 0)
		SetEvent(no_readers_);
}

void RWLock::WriterEnter() {
	// �赲����д��
	write_lock_.Lock();
	// �ȴ����ж�����ɶ�ȡ
	WaitForSingleObject(no_readers_, INFINITE);
}

void RWLock::WriterLeave() {
	write_lock_.Unlock();
}

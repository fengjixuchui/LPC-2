#pragma once
#include "critical_section.h"

//! ��д��, �����д
class RWLock {
public:
	RWLock();
	~RWLock();

	//! �������
	void ReaderEnter();
	//! �뿪����
	void ReaderLeave();

	//! ����д��
	void WriterEnter();
	//! �뿪д��
	void WriterLeave();

private:
	CCriticalSection write_lock_;
	CCriticalSection read_lock_;
	HANDLE no_readers_;
	int reader_count_;
};

class ReadLockGuard {
public:
	ReadLockGuard(RWLock *lock) : lock_(lock) {
		lock_->ReaderEnter();
	}
	~ReadLockGuard() {
		lock_->ReaderLeave();
	}

private:
	RWLock *lock_;
};

class WriteLockGuard {
public:
	WriteLockGuard(RWLock *lock) : lock_(lock) {
		lock_->WriterEnter();
	}
	~WriteLockGuard() {
		lock_->WriterLeave();
	}

private:
	RWLock *lock_;
};
#pragma once
#include "critical_section.h"

//! ¶ÁÐ´Ëø, ¶à¶Áµ¥Ð´
class RWLock {
public:
	RWLock();
	~RWLock();

	//! ½øÈë¶ÁËø
	void ReaderEnter();
	//! Àë¿ª¶ÁËø
	void ReaderLeave();

	//! ½øÈëÐ´Ëø
	void WriterEnter();
	//! Àë¿ªÐ´Ëø
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
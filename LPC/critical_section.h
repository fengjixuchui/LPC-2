#pragma once
#include "windows.h"

//! CRITICAL_SECTION�ļ򵥷�װ
class CCriticalSection {
private:
	CRITICAL_SECTION cs_;

public:
	CCriticalSection() {
		InitializeCriticalSection(&cs_);
	}

	~CCriticalSection() {
		DeleteCriticalSection(&cs_);
	}

	void Lock() {
		EnterCriticalSection(&cs_);
	}

	void Unlock() {
		LeaveCriticalSection(&cs_);
	}
};

//! CRITICAL_SECTION��RALL��װ
class CSGuard {
private:
	CCriticalSection *cs_;

public:
	CSGuard(CCriticalSection *cs) : cs_(cs) {
		cs_->Lock();
	}
	~CSGuard() {
		cs_->Unlock();
	}
};
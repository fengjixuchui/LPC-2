#include "rwlock.h"
#include "helpers.h"
RWLock::RWLock() {
	reader_count_ = 0;
	no_readers_ = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (no_readers_ == NULL)
		throw std::runtime_error("RWLock中CreateEvent错误, 错误码 = " + 
		I2String(GetLastError()));
}

RWLock::~RWLock() {
	WaitForSingleObject(no_readers_, INFINITE);
	CloseHandle(no_readers_);
}

void RWLock::ReaderEnter() {
	// 阻挡写者
	CSGuard w(&write_lock_);
	// 保证状态转换的原子性
	CSGuard r(&read_lock_);
	// 如果之前没有读者, 则no_readers_是设置状态, 需要重置使写者等待
	if (++reader_count_ == 1)
		ResetEvent(no_readers_);
}

void RWLock::ReaderLeave() {
	// 保证状态转换的原子性
	CSGuard r(&read_lock_);
	// 如果没有读者了, 设置no_readers_使写者写入
	if (--reader_count_ == 0)
		SetEvent(no_readers_);
}

void RWLock::WriterEnter() {
	// 阻挡其他写者
	write_lock_.Lock();
	// 等待所有读者完成读取
	WaitForSingleObject(no_readers_, INFINITE);
}

void RWLock::WriterLeave() {
	write_lock_.Unlock();
}

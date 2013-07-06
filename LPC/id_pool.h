#pragma once
#include <vector>
#include "critical_section.h"

class IDPool {
public:
	typedef unsigned int IDType;

	//! 获取一个空闲id
	int Get();
	//! 回收一个id
	void Put(int id);
	//! 占用一个id
	void Occupy(int id);

private:
	std::vector<int> ids_;
	CCriticalSection lock_;
};
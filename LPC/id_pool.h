#pragma once
#include <vector>
#include "critical_section.h"

class IDPool {
public:
	typedef unsigned int IDType;

	//! ��ȡһ������id
	int Get();
	//! ����һ��id
	void Put(int id);
	//! ռ��һ��id
	void Occupy(int id);

private:
	std::vector<int> ids_;
	CCriticalSection lock_;
};
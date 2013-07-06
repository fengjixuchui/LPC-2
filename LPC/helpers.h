#pragma once
#include <string>

//! 比较两个数的大小, 大的放在max中, 小的放在min中
template<typename T> void MaxMin(T &max, T &min) {
	if (max < min) {
		T tmp(std::move(max));
		max = std::move(min);
		min = std::move(tmp);
	}
}

std::string I2String(int val);
std::wstring I2WString(int val);
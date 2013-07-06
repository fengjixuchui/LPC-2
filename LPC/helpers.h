#pragma once
#include <string>

//! �Ƚ��������Ĵ�С, ��ķ���max��, С�ķ���min��
template<typename T> void MaxMin(T &max, T &min) {
	if (max < min) {
		T tmp(std::move(max));
		max = std::move(min);
		min = std::move(tmp);
	}
}

std::string I2String(int val);
std::wstring I2WString(int val);
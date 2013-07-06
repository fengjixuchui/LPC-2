#include "helpers.h"
#include <sstream>

std::string I2String(int val) {
	std::stringstream ss;
	ss << val;
	return ss.str();
}

std::wstring I2WString(int val) {
	std::wstringstream ss;
	ss << val;
	return ss.str();
}
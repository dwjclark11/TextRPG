#pragma once
#include <vector>
#include <string>
#include <Windows.h>

template <typename T>
void remove(std::vector<T>& vec, size_t index)
{
	vec.erase(vec.begin() + index);
}

std::wstring CharToWide(const char* str);
std::string WideToStr(const std::wstring& wstr);
std::wstring PadNumbers(int num);
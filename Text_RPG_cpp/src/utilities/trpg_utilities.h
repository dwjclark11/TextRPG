#pragma once
#include <vector>

template <typename T>
void remove(std::vector<T>& vec, size_t index)
{
	vec.erase(vec.begin() + index);
}
#pragma once
#include "pch.h"

namespace Util{
template <typename T>
static bool vectorContains(std::vector<T> vector, T element){
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}
}
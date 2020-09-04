#pragma once
#include "pch.h"
#include "ScreamAPI.h"

namespace Util{
template <typename T>
static bool vectorContains(std::vector<T> vector, T element){
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

std::filesystem::path getDLLparentDir(HMODULE hModule);

EOS_HPlatform getHPlatform();
EOS_HAuth getHAuth();
EOS_HConnect getHConnect();
EOS_HAchievements getHAchievements();
EOS_EpicAccountId getEpicAccountId();
EOS_ProductUserId getProductUserId();
char* copy_c_string(const char* c_string);

}

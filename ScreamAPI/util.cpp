#include "pch.h"
#include "util.h"
#include <eos-sdk\eos_auth.h>

namespace Util{

std::filesystem::path getDLLparentDir(HMODULE hModule){
	WCHAR modulePathBuffer[MAX_PATH];
	GetModuleFileName(hModule, modulePathBuffer, MAX_PATH);

	std::filesystem::path modulePath = modulePathBuffer;
	return modulePath.parent_path();
}

EOS_HPlatform getHPlatform(){
	return ScreamAPI::hPlatform;
}

EOS_HAuth getHAuth(){
	static auto result = EOS_Platform_GetAuthInterface(getHPlatform());
	return result;
}

EOS_HConnect getHConnect(){
	static auto result = EOS_Platform_GetConnectInterface(getHPlatform());
	return result;
}

EOS_HAchievements getHAchievements(){
	static auto result = EOS_Platform_GetAchievementsInterface(getHPlatform());
	return result;
}

EOS_EpicAccountId getEpicAccountId(){
	static auto result = EOS_Auth_GetLoggedInAccountByIndex(getHAuth(), 0);
	return result;
}

EOS_ProductUserId getProductUserId(){
	static auto result = EOS_Connect_GetLoggedInUserByIndex(getHConnect(), 0);
	return result;
}

}

#include "pch.h"
#include "eos-sdk/eos_sdk.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(EOS_HAchievements) EOS_Platform_GetAchievementsInterface(EOS_HPlatform Handle){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_GetAchievementsInterface, __func__);
	auto result = proxy(Handle);

	// Save the EOS_HAchievements handle for later use
	ScreamAPI::hAchievements = result;

	return result;
}

// A proxy implementation so that we can call this functions ourselves
EOS_DECLARE_FUNC(EOS_HConnect) EOS_Platform_GetConnectInterface(EOS_HPlatform Handle){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_GetConnectInterface, __func__);
	return proxy(Handle);
}
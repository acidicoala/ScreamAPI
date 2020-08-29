#include "pch.h"
#include "eos-sdk/eos_sdk.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(EOS_HAuth) EOS_Platform_GetAuthInterface(EOS_HPlatform Handle){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_GetAuthInterface, __func__);
	return proxy(Handle);
}

EOS_DECLARE_FUNC(EOS_HAchievements) EOS_Platform_GetAchievementsInterface(EOS_HPlatform Handle){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_GetAchievementsInterface, __func__);
	return proxy(Handle);
}

EOS_DECLARE_FUNC(EOS_HConnect) EOS_Platform_GetConnectInterface(EOS_HPlatform Handle){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_GetConnectInterface, __func__);
	return proxy(Handle);
}
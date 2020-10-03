#include "pch.h"
#include "eos-sdk/eos_sdk.h"
#include "ScreamAPI.h"

EOS_DECLARE_FUNC(EOS_HAuth) EOS_Platform_GetAuthInterface(EOS_HPlatform Handle){
	EOS_IMPLEMENT_FUNC(EOS_Platform_GetAuthInterface, Handle);
}

EOS_DECLARE_FUNC(EOS_HAchievements) EOS_Platform_GetAchievementsInterface(EOS_HPlatform Handle){
	EOS_IMPLEMENT_FUNC(EOS_Platform_GetAchievementsInterface, Handle);
}

EOS_DECLARE_FUNC(EOS_HConnect) EOS_Platform_GetConnectInterface(EOS_HPlatform Handle){
	EOS_IMPLEMENT_FUNC(EOS_Platform_GetConnectInterface, Handle);
}

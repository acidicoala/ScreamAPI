#include "pch.h"
#include "eos-sdk/eos_connect.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"

EOS_DECLARE_FUNC(void) EOS_Connect_Login(EOS_HConnect Handle, const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Connect_Login, __func__);
	auto container = new ScreamAPI::OriginalDataContainer(ClientData, CompletionDelegate);
	proxy(Handle, Options, container, [](const EOS_Connect_LoginCallbackInfo* Data){
		ScreamAPI::proxyCallback<EOS_Connect_LoginCallbackInfo>(Data, &Data->ClientData,
			[](EOS_Connect_LoginCallbackInfo* Data){
			if(Data->ResultCode == EOS_EResult::EOS_Success)
				AchievementManager::init();
		});
	});
}


EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_Connect_GetLoggedInUserByIndex(EOS_HConnect Handle, int32_t Index){
	EOS_IMPLEMENT_FUNC(EOS_Connect_GetLoggedInUserByIndex, Handle, Index)
}

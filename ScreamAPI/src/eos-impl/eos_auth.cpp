#include "pch.h"
#include "eos-sdk/eos_auth.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"
#include <future>

EOS_DECLARE_FUNC(void) EOS_Auth_Login(EOS_HAuth Handle, const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Auth_Login, __func__);
	//auto container = new AuthLoginContainer{ ClientData, CompletionDelegate }; // Don't forget to free the heap
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Auth_GetLoggedInAccountByIndex(EOS_HAuth Handle, int32_t Index){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Auth_GetLoggedInAccountByIndex, __func__);
	return proxy(Handle, Index);
}
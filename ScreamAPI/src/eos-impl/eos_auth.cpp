#include "pch.h"
#include "eos-sdk/eos_auth.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"
#include <future>

// TODO: Not using it at all, except for logging.
EOS_DECLARE_FUNC(void) EOS_Auth_Login(EOS_HAuth Handle, const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate){
	EOS_IMPLEMENT_FUNC(EOS_Auth_Login, Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Auth_GetLoggedInAccountByIndex(EOS_HAuth Handle, int32_t Index){
	EOS_IMPLEMENT_FUNC(EOS_Auth_GetLoggedInAccountByIndex, Handle, Index);
}
#include "pch.h"
#include "eos-sdk/eos_common.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result){
	EOS_IMPLEMENT_FUNC(EOS_EResult_ToString, Result);
}

EOS_DECLARE_FUNC(EOS_Bool) EOS_EpicAccountId_IsValid(EOS_EpicAccountId AccountId){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_EpicAccountId_IsValid, __func__);
	auto result = proxy(AccountId);

	Logger::debug("EpicAccountId_IsValid: %s", result ? "True" : "False");

	return result;
}

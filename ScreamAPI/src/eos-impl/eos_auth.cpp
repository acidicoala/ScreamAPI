#include "pch.h"
#include "eos-sdk/eos_auth.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Auth_GetLoggedInAccountByIndex(EOS_HAuth Handle, int32_t Index){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Auth_GetLoggedInAccountByIndex, __func__);
	auto result = proxy(Handle, Index);

	// Save the EOS_EpicAccountId handle for later use
	ScreamAPI::EpicAccountId = result;

	return result;
}
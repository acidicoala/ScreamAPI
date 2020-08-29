#include "pch.h"
#include "eos-sdk/eos_connect.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_Connect_GetLoggedInUserByIndex(EOS_HConnect Handle, int32_t Index){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Connect_GetLoggedInUserByIndex, __func__);
	auto result = proxy(Handle, Index);

	// Save the EOS_ProductUserId handle for later use
	ScreamAPI::ProductUserId = result;

	return result;
}
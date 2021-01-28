#include "pch.h"
#include "eos-sdk/eos_common.h"
#include "ScreamAPI.h"
#include "util.h"

EOS_DECLARE_FUNC(EOS_Bool) EOS_EpicAccountId_IsValid(EOS_EpicAccountId AccountId){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_EpicAccountId_IsValid, __func__);
	auto result = proxy(AccountId);

	Logger::debug("EpicAccountId_IsValid: %s", result ? "True" : "False");

	return result;
}

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result){
	try{
		static auto proxy = ScreamAPI::proxyFunction(&EOS_EResult_ToString, __func__);
		return proxy(Result);
	} catch(ScreamAPI::FunctionNotFoundException){
		// Was there a similar function in legacy releases?
		return Util::copy_c_string((std::to_string((int)Result).c_str()));
	}
}

#include "pch.h"
#include "eos-sdk/eos_common.h"
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_EResult_ToString, __func__);
	return proxy(Result);
}
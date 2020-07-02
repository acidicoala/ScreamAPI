#include "pch.h"
#include <eos-sdk\eos_types.h>
#include <ScreamAPI.h>

EOS_DECLARE_FUNC(void) EOS_Platform_Tick(EOS_HPlatform Handle){
	ScreamAPI::runCallbacks();

	static auto proxied = ScreamAPI::proxyFunction<decltype(EOS_Platform_Tick)>(__FUNCTION__);
	proxied(Handle);
}
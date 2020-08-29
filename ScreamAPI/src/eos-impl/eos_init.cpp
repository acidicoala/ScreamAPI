#include "pch.h"
#include "eos-sdk/eos_init.h"
#include <ScreamAPI.h>
#include <achievement_manager.h>

EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_Create, __func__);
	auto result = proxy(Options);

	// Save the EOS_HPlatform handle for later use
	ScreamAPI::hPlatform = result;

	return result;
}

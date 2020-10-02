#include "pch.h"
#include "eos-sdk/eos_init.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"
#include "util.h"

EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options){
	if(Config::ForceEpicOverlay()){
		// FIXME: This may lead to a crash with future EOS SDK headers or old dlls...
		Logger::debug("[EOS_Platform_Options]");
		Logger::debug("\t""ApiVersion: %d", Options->ApiVersion);
		Logger::debug("\t""Flags: %d", Options->Flags);

		auto mOptions = const_cast<EOS_Platform_Options*>(Options);
		mOptions->Flags = 0;
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Platform_Create, __func__);
	auto result = proxy(Options);

	// Save the EOS_HPlatform handle for later use
	Util::hPlatform = result;

	return result;
}

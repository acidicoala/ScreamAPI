#include "pch.h"
#include "ScreamAPI.h"
#include "constants.h"
#include "eos-sdk/eos_achievements.h"
#include "util.h"
#include <Overlay.h>

using namespace Util;

namespace ScreamAPI{

// Initialize extern variables
HMODULE thisDLL = nullptr;
HMODULE originalDLL = nullptr;
bool isScreamAPIinitialized = false;

void ScreamAPI::init(HMODULE hModule){
	// Check if DLL is already initialized
	if(isScreamAPIinitialized) // Do we really need to check that?
		return;
	else
		isScreamAPIinitialized = true;

	thisDLL = hModule;

	// Initialize Config
	auto iniPath = getDLLparentDir(hModule) / SCREAM_API_CONFIG;
	Config::init(iniPath.generic_wstring());

	// Initialize Logger
	auto logPath = getDLLparentDir(hModule) / Config::LogFilename();
	Logger::init(Config::EnableLogging(),
				 Config::LogDLCQueries(),
				 Config::LogAchievementQueries(),
				 Config::LogOverlay(),
				 Config::LogLevel(),
				 logPath.generic_wstring());

	Logger::info("ScreamAPI v" SCREAM_API_VERSION);

	// Load original library
	auto orinalDLLpath = getDLLparentDir(hModule) / SCREAM_API_ORIG_DLL;
	originalDLL = LoadLibrary(orinalDLLpath.generic_wstring().c_str());
	if(originalDLL)
		Logger::info("Successfully loaded original EOS SDK: %s", SCREAM_API_ORIG_DLL);
	else
		Logger::error("Failed to load original EOS SDK: %s", SCREAM_API_ORIG_DLL);
}


void proxyCallback(void* callbackInfoData, void** clientData, std::function<void()> customCallback){
	auto container = reinterpret_cast<OriginalDataContainer*>(*clientData);

	// Restore original client data
	*clientData = container->originalClientData;

	customCallback();

	// Call original completion delegate with our modified data
	container->originalCompletionDelegate(callbackInfoData);

	// Free the heap
	delete container;
}

void ScreamAPI::destroy(){
	Logger::info("Game requested to free the EOS SDK");
	Overlay::Shutdown();
	FreeLibrary(originalDLL);
	isScreamAPIinitialized = false;
}

}

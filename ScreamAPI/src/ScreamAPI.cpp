#include "pch.h"
#include "ScreamAPI.h"
#include "constants.h"
#include "eos-sdk\eos_achievements.h"
#include "util.h"
#include <Overlay.h>

using namespace Util;

namespace ScreamAPI{

// Initialize extern variables
HMODULE thisDLL = nullptr;
HMODULE originalDLL = nullptr;
EOS_HPlatform hPlatform = nullptr;
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

// Checks against Options->ApiVersion and warns user if version is newer
void ScreamAPI::checkSdkVersion(const int32_t apiVersion, const int32_t maxVersion){
	if(apiVersion > maxVersion){
		static bool warned = false; // Warn only once
		if(!warned){
			Logger::warn("Game uses newer EOS SDK (%d) than what ScreamAPI is built for (%d)!", apiVersion, maxVersion);
			warned = true;
		}
	}
}

void ScreamAPI::destroy(){
	Logger::info("Game requested to free the EOS SDK");
	Overlay::Shutdown();
	FreeLibrary(originalDLL);
	isScreamAPIinitialized = false;
}

}
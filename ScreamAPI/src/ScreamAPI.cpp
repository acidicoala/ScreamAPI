#include "pch.h"
#include "ScreamAPI.h"
#include "constants.h"
#include <Overlay.h>
#include <eos-sdk\eos_achievements.h>
#include <util.h>

using namespace Util;

namespace ScreamAPI{

// Initialize extern variables
bool isDLLinitialized = false;
HMODULE originalDLL = nullptr;

Achievements achievements;

// TODO: Encapsulate these in a struct?
EOS_HPlatform hPlatform = nullptr;
EOS_HAchievements hAchievements = nullptr;
EOS_EpicAccountId EpicAccountId = nullptr;
EOS_ProductUserId ProductUserId = nullptr;

void ScreamAPI::init(HMODULE hModule){
	// Check if DLL is already initialized
	if(isDLLinitialized) // Do we really need to check that?
		return;
	else
		isDLLinitialized = true;

	// Initialize Config
	auto iniPath = getDLLparentDir(hModule) / SCREAM_API_CONFIG;
	Config::init(iniPath.generic_wstring());

	// Initialize Logger
	auto logPath = getDLLparentDir(hModule) / Config::getLogFilename();
	Logger::init(Config::isLogEnabled(),
				 Config::isLoggingDLCQueries(),
				 Config::isLoggingAchievements(),
				 Config::isLoggingOverlay(),
				 Config::getLogLevel(),
				 logPath.generic_wstring());

	Logger::info("ScreamAPI v" SCREAM_API_VERSION);

	// Initialize Overlay
	Overlay::init(hModule, achievements, unlockAchievement);

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
	Overlay::shutdown();

}

}
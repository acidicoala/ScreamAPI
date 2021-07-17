#include "pch.h"
#include "ScreamAPI.h"
#include "constants.h"
#include "util.h"
#include <Overlay.h>

using namespace Util;

namespace ScreamAPI
{
	// Initialize extern variables
	HMODULE thisDLL = nullptr;
	HMODULE originalDLL = nullptr;
	bool isScreamAPIinitialized = false;

	void init(HMODULE hModule) {
		// Check if DLL is already initialized
		if (isScreamAPIinitialized) // Do we really need to check that?
			return;

		isScreamAPIinitialized = true;

		thisDLL = hModule;

		// Initialize Config
		const auto iniPath = getDLLparentDir(hModule) / SCREAM_API_CONFIG;
		Config::init(iniPath.generic_wstring());

		// Initialize Logger
		const auto logPath = getDLLparentDir(hModule) / Config::LogFilename();
		Logger::init(Config::EnableLogging(),
		             Config::LogDLCQueries(),
		             Config::LogAchievementQueries(),
		             Config::LogOverlay(),
		             Config::LogLevel(),
		             logPath.generic_wstring());

		Logger::info("ScreamAPI v" SCREAM_API_VERSION);

		// Load original library
		const auto originalDllPath = getDLLparentDir(hModule) / SCREAM_API_ORIG_DLL;
		originalDLL = LoadLibrary(originalDllPath.generic_wstring().c_str());
		if (originalDLL)
			Logger::info("Successfully loaded original EOS SDK: %s", SCREAM_API_ORIG_DLL);
		else
			Logger::error("Failed to load original EOS SDK: %s", SCREAM_API_ORIG_DLL);
	}


	void destroy() {
		Logger::info("Game requested to shutdown the EOS SDK");
		if (Config::EnableOverlay()) {
			Overlay::Shutdown();
		}
		FreeLibrary(originalDLL);
		isScreamAPIinitialized = false;
	}
}

#include "pch.h"
#include "ScreamAPI.h"
#include "Constants.h"

// Initialize static variables
bool ScreamAPI::isDLLinitialized = false;
HMODULE ScreamAPI::originalDLL = nullptr;
std::mutex ScreamAPI::callbackMutex;
std::deque<std::function<void()>> ScreamAPI::callbacks;

// Utility variables & functions

static std::filesystem::path getDLLparentDir(HMODULE hModule){
	WCHAR ModulePathStr[4096]; // TODO: figure out buffer size dynamically?
	GetModuleFileNameW(hModule, ModulePathStr, 4096);

	std::filesystem::path ModulePath = ModulePathStr;
	return ModulePath.parent_path();
}

// Public functions

void ScreamAPI::init(HMODULE hModule){
	// Check if DLL is already initialized
	if(isDLLinitialized) // Do we really need to check that?
		return;
	else
		isDLLinitialized = true;

	// Initialize config
	auto iniPath = getDLLparentDir(hModule) / SCREAM_API_CONFIG;
	Config::init(iniPath.generic_wstring());

	// Initialize logger
	auto logPath = getDLLparentDir(hModule) / Config::getLogFilename();
	Logger::init(Config::isLogEnabled(), Config::getLogLevel(), logPath.generic_string());

	Logger::info("ScreamAPI v" SCREAM_API_VERSION);

	// Load original library
	auto orinalDLLpath = getDLLparentDir(hModule) / SCREAM_API_ORIG_DLL;
	originalDLL = LoadLibrary(orinalDLLpath.c_str());
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

void ScreamAPI::addCallback(std::function<void()> func){
	std::lock_guard<std::mutex> guard(callbackMutex);
	callbacks.push_back(func);
}

void ScreamAPI::runCallbacks(){
	std::unique_lock<std::mutex> lock(callbackMutex);
	while(!callbacks.empty()){
		auto callbackFunction = callbacks.front();
		callbacks.pop_front();
		lock.unlock(); // unlock before running callback, so callback is able to modify callback vector
		callbackFunction();
		lock.lock(); // relock afterward
	}
}
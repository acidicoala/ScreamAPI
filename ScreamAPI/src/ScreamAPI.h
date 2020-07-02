#pragma once
#include "Logger.h"
#include "Config.h"

class ScreamAPI{
private:
	static bool isDLLinitialized;
	static HMODULE originalDLL;
	static std::mutex callbackMutex;
	static std::deque<std::function<void()>> callbacks;
public:
	static void init(HMODULE hModule);
	static void checkSdkVersion(const int32_t apiVersion, const int32_t maxVersion);
	static void addCallback(std::function<void()> func);
	static void runCallbacks();

	// Have to define template in header (-_-)
	template<typename signature>
	static std::function<signature> proxyFunction(LPCSTR functionName){
		auto funcPtr = GetProcAddress(originalDLL, functionName);
		if(!funcPtr)
			Logger::error("Failed to proxy function: %s", functionName);
		return std::function<signature>(reinterpret_cast<signature*>(funcPtr));
	}
};


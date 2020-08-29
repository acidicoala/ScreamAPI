#pragma once
#include "pch.h"

class Config{
public:
	static void init(const std::wstring iniPath);
	static bool isUnlockingAllDLC();
	static bool isProxyingEntitlements();
	static bool isLogEnabled();
	static bool isLoggingDLCQueries();
	static bool isLoggingAchievements();
	static bool isLoggingOverlay();
	static std::string getLogLevel();
	static std::string getLogFilename();
	static const std::vector<std::string>& getOwnedItemIDs();
};
#pragma once
#include "pch.h"

namespace Config{

void init(const std::wstring iniPath);

// ScreamAPI
bool EnableItemUnlocker();
bool EnableEntitlementUnlocker();
bool EnableLogging();
bool EnableOverlay();
// Logging
std::string LogLevel();
std::string LogFilename();
bool LogDLCQueries();
bool LogAchievementQueries();
bool LogOverlay();
// Overlay
bool LoadIcons();
bool CacheIcons();
bool ValidateIcons();
// DLC
bool UnlockAllDLC();
// DLC_List
std::vector<std::string> DLC_List();

};
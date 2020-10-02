#include "pch.h"
#include "Config.h"
#include "ConfigUtils.h"
#include <map>

namespace Config{

// ScreamAPI
bool bEnableItemUnlocker = true;
bool bEnableEntitlementUnlocker = true;
bool bEnableLogging = false;
bool bEnableOverlay = false;
// Logging
std::string sLogLevel = "INFO";
std::string sLogFilename = "ScreamAPI.log";
bool bLogDLCQueries = true;
bool bLogAchievementQueries = false;
bool bLogOverlay = false;
// Overlay
bool bLoadIcons = true;
bool bCacheIcons = true;
bool bValidateIcons = true;
bool bForceEpicOverlay = false;
// DLC
bool bUnlockAllDLC = true;
bool bOfflineSupport = true;
// DLC_List
std::vector<std::string> vDLC_List;

std::map<std::string, std::map<std::string, void*>> configMap = {
	{"ScreamAPI", {
		{"EnableItemUnlocker", &bEnableItemUnlocker},
		{"EnableEntitlementUnlocker", &bEnableEntitlementUnlocker},
		{"EnableLogging", &bEnableLogging},
		{"EnableOverlay", &bEnableOverlay},
	}},
	{"Logging", {
		{"LogLevel", &sLogLevel},
		{"LogFilename", &sLogFilename},
		{"LogDLCQueries", &bLogDLCQueries},
		{"LogAchievementQueries", &bLogAchievementQueries},
		{"LogOverlay", &bLogOverlay}
	}},
	{"Overlay",{
		{"LoadIcons", &bLoadIcons},
		{"CacheIcons", &bCacheIcons},
		{"ValidateIcons", &bValidateIcons},
		{"ForceEpicOverlay", &bForceEpicOverlay},
	}},
	{"DLC", {
		{"UnlockAllDLC", &bUnlockAllDLC},
		{"OfflineSupport", &bOfflineSupport},
	}},
};

int iniHandler(void* user, const char* section_raw, const char* name_raw, const char* value_raw){
	std::string section = section_raw;
	std::string name = name_raw;
	std::string value = value_raw;

	try{
		if(section == "DLC_List"){
			if(stringToBool(value)) // Add to the list only IDs set to True
				vDLC_List.push_back(name);
			return TRUE;
		} else{
			try{
				auto sectionMap = configMap.at(section);
				try{
					auto varPtr = sectionMap.at(name);

					// If the config line is a string
					if(name == "LogLevel" || name == "LogFilename"){
						auto stringPtr = static_cast<std::string*>(varPtr);
						*stringPtr = value;
					} else{
						// Else the config line is a bool
						auto boolPtr = static_cast<bool*>(varPtr);
						*boolPtr = stringToBool(value);
					}
					return TRUE;
				} catch(std::out_of_range&){
					showError("Invalid name (" + name + ") at section [" + section + "]");
					return FALSE;
				}

			} catch(std::out_of_range&){
				showError("Invalid section name: " + section);
				return FALSE;
			}
		}
	} catch(InvalidBoolValue& ex){
		showError("Invalid boolean value (" + ex.value + ") for name [" + name + "]");
		return FALSE;
	}
}

void init(const std::wstring iniPath){
	int parseResult = ini_wparse(iniPath.c_str(), iniHandler, 0);

	if(parseResult != 0)
		exit(1);
}

// ScreamAPI
bool EnableItemUnlocker(){ return bEnableItemUnlocker; }
bool EnableEntitlementUnlocker(){ return bEnableEntitlementUnlocker; }
bool EnableOverlay(){ return bEnableOverlay; }
// Logging
bool EnableLogging(){ return bEnableLogging; }
std::string LogLevel(){ return sLogLevel; }
std::string LogFilename() { return sLogFilename; }
bool LogDLCQueries(){ return bLogDLCQueries; }
bool LogAchievementQueries(){ return bLogAchievementQueries; }
bool LogOverlay(){ return bLogOverlay; }
// Overlay
bool LoadIcons() { return bLoadIcons; }
bool CacheIcons() { return bCacheIcons; }
bool ValidateIcons(){ return bValidateIcons; }
bool ForceEpicOverlay(){ return bForceEpicOverlay; }
// DLC
bool UnlockAllDLC(){ return bUnlockAllDLC; }
bool OfflineSupport(){ return bOfflineSupport; }
// DLC_List
std::vector<std::string> DLC_List(){ return vDLC_List; }

}

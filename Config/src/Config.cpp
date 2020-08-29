#include "pch.h"
#include "Config.h"
#include "ConfigUtils.h"

bool unlockAllDLC = true;
bool logIsEnabled = false;
bool logDLCQueries = true;
bool logAchievements = false;
bool logOverlay = false;
bool proxyEntitlements = false;
std::string logLevel = "INFO";
std::string logFilename = "ScreamAPI.log";
std::vector<std::string> ownedItemIDs;

int iniHandler(void* user, const char* section_raw, const char* name_raw, const char* value){
	std::string section = section_raw;
	std::string name = name_raw;

	try{
		if(section == "ScreamAPI"){
			if(name == "UnlockAllDLC")
				unlockAllDLC = stringToBool(value);
			else if(name == "ProxyEntitlements")
				proxyEntitlements = stringToBool(value);
			else
				throw InvalidSectionName(section_raw, name_raw);
		} else if(section == "Logging"){
			if(name == "Logging")
				logIsEnabled = stringToBool(value);
			else if(name == "LogFile")
				logFilename = value;
			else if(name == "LogLevel")
				logLevel = value;
			else if(name == "LogDLCQueries")
				logDLCQueries = stringToBool(value);
			else if(name == "LogAchievements")
				logAchievements = stringToBool(value);
			else if(name == "LogOverlay")
				logOverlay = stringToBool(value);
			else
				throw InvalidSectionName(section_raw, name_raw);
		} else if(section == "DLC"){
			ownedItemIDs.push_back(name);
		} else{
			showError("Invalid section name: " + section);
			return 0;
		}

		return 1;
	} catch(InvalidBoolValue& ex){
		showError("Invalid boolean value (" + ex.value + ") for name [" + name + "]");
		return 0;
	} catch(InvalidSectionName& ex){
		showError("Invalid name (" + ex.name + ") at section [" + ex.section + "]");
		return 0;
	}
}

void Config::init(const std::wstring iniPath){
	int parseResult = ini_wparse(iniPath.c_str(), iniHandler, 0);

	if(parseResult > 0)
		showError(L"Error reading " + iniPath + L"\nError on line: " + std::to_wstring(parseResult) + L"\nUsing defaults");
	//else if(parseResult < 0)
		//showError(L"Error opening " + iniPath + L"\nUsing defaults");
}

bool Config::isUnlockingAllDLC(){
	return unlockAllDLC;
}

bool Config::isProxyingEntitlements(){
	return proxyEntitlements;
}

bool Config::isLogEnabled(){
	return logIsEnabled;
}

bool Config::isLoggingDLCQueries(){
	return logDLCQueries;
}

bool Config::isLoggingAchievements(){
	return logAchievements;
}

bool Config::isLoggingOverlay(){
	return logOverlay;
}

std::string Config::getLogLevel(){
	return logLevel;
}

std::string Config::getLogFilename(){
	return logFilename;
}

const std::vector<std::string>& Config::getOwnedItemIDs(){
	return ownedItemIDs;
}

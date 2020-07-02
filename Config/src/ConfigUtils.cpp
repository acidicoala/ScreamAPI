#include "pch.h"
#include "ConfigUtils.h"

void showError(std::string message){
	showError(std::wstring(message.begin(), message.end()));
}

void showError(std::wstring message){
	MessageBox(NULL, message.c_str(), L"Config Error", MB_ICONERROR | MB_OK);
}

bool stringToBool(const char* val){
	std::string value = val;

	if(value == "True" || value == "true")
		return true;
	else if(value == "False" || value == "false")
		return false;
	else
		throw InvalidBoolValue(val);
}
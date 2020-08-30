#include "pch.h"
#include "ConfigUtils.h"

void showError(std::string message){
	showError(std::wstring(message.begin(), message.end()));
}

void showError(std::wstring message){
	MessageBox(NULL, message.c_str(), L"ScreamAPI Config Error", MB_ICONERROR | MB_OK);
}

bool stringToBool(std::string& value){
	if(value == "True" || value == "true")
		return true;
	else if(value == "False" || value == "false")
		return false;
	else
		throw InvalidBoolValue(value);
}
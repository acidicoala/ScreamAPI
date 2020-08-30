#pragma once
#include "pch.h"

void showError(std::string message);
void showError(std::wstring message);
bool stringToBool(std::string& value);

class InvalidSectionName: public std::exception{
public:
	std::string section;
	std::string name;
	InvalidSectionName(const char* section_raw, const char* name_raw) : section(section_raw), name(name_raw){}
};

class InvalidBoolValue: public std::exception{
public:
	std::string value;
	InvalidBoolValue(std::string& val) : value(val){}
};
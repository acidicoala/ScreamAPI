#pragma once
#include "pch.h"

class Logger{
public:
	static void init(bool logging, bool loggingDLC, std::string level, std::wstring logFilepath);

	static void debug(const char* const message, ...);
	static void info(const char* const message, ...);
	static void warn(const char* const message, ...);
	static void error(const char* const message, ...);
	static void dlc(const char* const message, ...);
};
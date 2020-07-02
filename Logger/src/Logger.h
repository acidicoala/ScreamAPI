#pragma once
#include "pch.h"

class Logger{
public:
	static void init(bool logging, std::string level, std::string logFilepath);

	static void debug(const char* const message, ...);
	static void info(const char* const message, ...);
	static void warn(const char* const message, ...);
	static void error(const char* const message, ...);
};
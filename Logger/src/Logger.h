#pragma once
#include "pch.h"

namespace Logger{
void init(bool logging, bool loggingDLC, bool loggingAch, bool loggingOvrl,
		  std::string level, std::wstring logFilepath);

void debug(const char* const message, ...);
void info(const char* const message, ...);
void warn(const char* const message, ...);
void error(const char* const message, ...);
void dlc(const char* const message, ...);
void ach(const char* const message, ...);
void ovrly(const char* const message, ...);
};
#include "pch.h"
#include "Logger.h"
#include <mutex>
#include <fstream>

enum class LogLevel{ DLC, ERR, WARN, INFO, DEBUG };
const char* const strLogLevels[] = {"DLC", "ERROR", "WARN", "INFO", "DEBUG"};

std::mutex logMutex;
bool isEnabled = false;
bool isLoggingDLCqueries = false;
LogLevel logLevel = LogLevel::INFO;
std::string logFilepath = "ScreamAPI.log";

void Logger::init(bool logging, bool loggingDLC, std::string level, std::string filepath){
	// Enable logging?
	isEnabled = logging;
	isLoggingDLCqueries = loggingDLC;

	// Convert the log level from string to Enum
	if(level == "DEBUG")
		logLevel = LogLevel::DEBUG;
	else if(level == "INFO")
		logLevel = LogLevel::INFO;
	else if(level == "WARN")
		logLevel = LogLevel::WARN;
	else if(level == "ERROR")
		logLevel = LogLevel::ERR;
	else{
		std::string message = "Invalid Log Level: " + level;
		std::wstring wmessage(message.begin(), message.end());
		MessageBox(NULL, wmessage.c_str(), L"Logger Init Error", MB_ICONERROR | MB_OK);
	}

	// Save the log file path
	logFilepath = filepath;

	if(isEnabled){
		// Clear the current log file, if it exists.
		std::ofstream ofs;
		ofs.open(logFilepath, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}
}

void log(LogLevel level, const char* const message, va_list args){
	// Do not log  if logging disabled
	if(!isEnabled)
		return;

	// Do not log if the level is below permitted
	if(level > logLevel)
		return;

	{ // Code block for lock_guard destructor to release lock
		std::lock_guard<std::mutex> guard(logMutex);

		// Format the message with provided arguments
		char* buffer = new char[4096];
		vsnprintf(buffer, 4096, message, args);

		std::ofstream file;
		file.open(logFilepath, std::ofstream::out | std::ofstream::app);
		if(!file.is_open())
			return; // Should not be the case, normally

		file << "[" << strLogLevels[(int) level] << "]\t";
		file << buffer << "\n";

		file.close();

		delete[] buffer;
	} // Mutex will be unlocked here
}

void Logger::debug(const char* const message, ...){
	va_list args;
	va_start(args, message);
	log(LogLevel::DEBUG, message, args);
	va_end(args);
}

void Logger::info(const char* const message, ...){
	va_list args;
	va_start(args, message);
	log(LogLevel::INFO, message, args);
	va_end(args);
}
void Logger::warn(const char* const message, ...){
	va_list args;
	va_start(args, message);
	log(LogLevel::WARN, message, args);
	va_end(args);
}

void Logger::error(const char* const message, ...){
	va_list args;
	va_start(args, message);
	log(LogLevel::ERR, message, args);
	va_end(args);
}

void Logger::dlc(const char* const message, ...){
	va_list args;
	va_start(args, message);
	if(isLoggingDLCqueries)
		log(LogLevel::DLC, message, args);
	va_end(args);
}


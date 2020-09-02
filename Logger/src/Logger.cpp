#include "pch.h"
#include "Logger.h"
#include <mutex>
#include <fstream>

enum class LogLevel{ OVRLY, ACH, DLC, ERR, WARN, INFO, DEBUG };
const char* const strLogLevels[] = {"OVRLY", "ACH", "DLC", "ERROR", "WARN", "INFO", "DEBUG"};

bool isEnabled = false;
bool isLoggingDLCqueries = false;
bool isLoggingAchievements = false;
bool isLoggingOverlay = false;
LogLevel logLevel = LogLevel::INFO;
std::wstring logFilepath = L"ScreamAPI.log";

void Logger::init(bool logging, bool loggingDLC, bool loggingAch, bool loggingOvrl,
				  std::string level, std::wstring filepath){
	// Enable logging?
	isEnabled = logging;
	isLoggingDLCqueries = loggingDLC;
	isLoggingAchievements = loggingAch;
	isLoggingOverlay = loggingOvrl;

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

	static std::mutex logMutex;
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

// I generally don't like using macros, but in this case I believe it's justified
// in order to avoid boilerplate code associated with varargs
#define DEFINE_LOGGER_FUNCTION(functionName, Level, condition)	\
void Logger::functionName(const char* const message, ...){		\
	va_list args;												\
	va_start(args, message);									\
	if(condition)												\
		log(Level, message, args);								\
	va_end(args);												\
}																\

DEFINE_LOGGER_FUNCTION(debug, LogLevel::DEBUG, true)
DEFINE_LOGGER_FUNCTION(info, LogLevel::INFO, true)
DEFINE_LOGGER_FUNCTION(warn, LogLevel::WARN, true)
DEFINE_LOGGER_FUNCTION(error, LogLevel::ERR, true)
DEFINE_LOGGER_FUNCTION(dlc, LogLevel::DLC, isLoggingDLCqueries)
DEFINE_LOGGER_FUNCTION(ach, LogLevel::ACH, isLoggingAchievements)
DEFINE_LOGGER_FUNCTION(ovrly, LogLevel::OVRLY, isLoggingOverlay)

#pragma once

// Windows headers
#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <windows.h>

// 3rd party headers
#include <spdlog/logger.h>               // spdlog::logger
#include <spdlog/sinks/basic_file_sink.h>// spdlog::basic_logger_mt
#include <spdlog/sinks/null_sink.h>      // spdlog::null_logger_mt
#include <spdlog/pattern_formatter.h>    // spdlog::custom_flag_formatter

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

// C++ Standard Library
#include <filesystem>// std::filesystem
#include <functional>// std::function
#include <memory>    // std::shared_ptr
#include <string>    // std::string | std::wstring
#include <utility>   // std::forward
#include <vector>    // std::vector
#include <algorithm> // std::find

// This definition turns import definitions into export definitions in EOS headers
#define EOS_BUILDING_SDK TRUE

using namespace fmt::literals; // "{}"_format() helper

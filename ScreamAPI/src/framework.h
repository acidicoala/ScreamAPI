#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude not needed win api definitions
#include <Windows.h>

// ScreamAPI.h & ScreamAPI.cpp
#include <filesystem>
#include <functional>
#include <sstream>
#include <vector>
#include <string>
#include <Logger.h>

// Ecom
#include "eos-sdk/eos_base.h"

// Include the linker exports based on the target architecture (32 vs 64)
// LegacyExports are special linker exports aimed to support older versions of EOS SDK
#ifdef _WIN64
#include "LinkerExports/LinkerExports64.h"
#include "LinkerExports/LegacyExports64.h"
#else
#include "LinkerExports/LinkerExports32.h"
#include "LinkerExports/LegacyExports32.h"
#endif

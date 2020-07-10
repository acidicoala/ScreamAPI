#pragma once
#define WIN32_LEAN_AND_MEAN // Exclude not needed win api definitions

#include <Windows.h>

// ScreamAPI.h & ScreamAPI.cpp
#include <filesystem>
#include <functional>
#include <sstream>

#include "eos-sdk/eos_base.h"

// Include the linker exports based on the target architecture (32 vs 64)
// LegacyExports are special linker exports aimed to support older versions of EOS SDK
#ifdef _WIN64
#include "LinkerExports64.h"
#include "LegacyExports64.h"
#else
#include "LinkerExports32.h"
#include "LegacyExports32.h"
#endif
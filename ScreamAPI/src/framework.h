#pragma once
#define NOMINMAX // Do we need this?
#define WIN32_LEAN_AND_MEAN // Exclude not needed win api definitions

#include <Windows.h>

// ScreamAPI.h & ScreamAPI.cpp
#include <filesystem>
#include <functional>
#include <mutex>
#include <deque>

#include "eos-sdk/eos_base.h"
#include "eos-sdk/eos_ecom.h" // for dlc

// Include the linker exports based on the target architecture (32 vs 64)
#ifdef _WIN64
#include "LinkerExports64.h"
#else
#include "LinkerExports32.h"
#endif
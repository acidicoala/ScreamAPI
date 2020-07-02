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
#include "eos-sdk/eos_sdk.h" // for ticks

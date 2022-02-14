#pragma once

#include "koalabox/pch.hpp"

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include <polyhook2/CapstoneDisassembler.hpp>
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/Detour/x64Detour.hpp>

// This definition turns import definitions into export definitions in EOS headers
#define EOS_BUILDING_SDK TRUE

#include "koalabox/dll_monitor/dll_monitor.hpp"
#include "koalabox/hook/hook.hpp"
#include "koalabox/loader/loader.hpp"
#include "koalabox/logger/logger.hpp"
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"

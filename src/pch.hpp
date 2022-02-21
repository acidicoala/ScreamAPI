#pragma once

#include "koalabox/pch.hpp"

#include <cpr/cpr.h>

#include "3rd_party/json.hpp"
#include "3rd_party/polyhook2.hpp"

// This definition turns import definitions into export definitions in EOS headers
#define EOS_BUILDING_SDK TRUE

#include "koalabox/config/config_parser.hpp"
#include "koalabox/dll_monitor/dll_monitor.hpp"
#include "koalabox/hook/hook.hpp"
#include "koalabox/loader/loader.hpp"
#include "koalabox/file_logger/file_logger.hpp"
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"

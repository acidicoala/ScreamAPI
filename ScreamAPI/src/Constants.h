#pragma once

#define SCREAM_API_VERSION "1.6.2" // Same as EOSSDK
#define SCREAM_API_CONFIG "ScreamAPI.ini"

#ifdef _WIN64
#define SCREAM_API_ORIG_DLL "EOSSDK-Win64-Shipping_o.dll"
#else
#define SCREAM_API_ORIG_DLL "EOSSDK-Win32-Shipping_o.dll"
#endif
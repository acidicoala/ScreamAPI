#pragma once

#define SCREAM_API_VERSION "1.7.0-2" // format: <EOSSDK version>-<ScreamAPI version for that EOSSDK>
#define SCREAM_API_CONFIG "ScreamAPI.ini"

// Define original dll path based on the target architecture (32 vs 64)
#ifdef _WIN64
#define SCREAM_API_ORIG_DLL "EOSSDK-Win64-Shipping_o.dll"
#else
#define SCREAM_API_ORIG_DLL "EOSSDK-Win32-Shipping_o.dll"
#endif
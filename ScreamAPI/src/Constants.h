#pragma once

const char* SCREAM_API_CONFIG = "ScreamAPI.ini";

// Define original dll path based on the target architecture (32 vs 64)
#ifdef _WIN64
const char* SCREAM_API_ORIG_DLL = "EOSSDK-Win64-Shipping_o.dll";
#else
const char* SCREAM_API_ORIG_DLL = "EOSSDK-Win32-Shipping_o.dll";
#endif
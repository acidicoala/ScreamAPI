#pragma once

/**
 * Collection of function exports that were at some point discontinued in EOS SDK
 */

#ifdef _WIN64

#pragma comment(linker, "/export:EOS_AccountId_FromString=EOSSDK-Win64-Shipping_o.EOS_AccountId_FromString")
#pragma comment(linker, "/export:EOS_AccountId_IsValid=EOSSDK-Win64-Shipping_o.EOS_AccountId_IsValid")
#pragma comment(linker, "/export:EOS_AccountId_ToString=EOSSDK-Win64-Shipping_o.EOS_AccountId_ToString")

#else

#pragma comment(linker, "/export:_EOS_AccountId_FromString@4=EOSSDK-Win32-Shipping_o._EOS_AccountId_FromString@4=")
#pragma comment(linker, "/export:_EOS_AccountId_IsValid@4=EOSSDK-Win32-Shipping_o._EOS_AccountId_IsValid@4=")
#pragma comment(linker, "/export:_EOS_AccountId_ToString@12=EOSSDK-Win32-Shipping_o_EOS_AccountId_ToString@12")

#endif

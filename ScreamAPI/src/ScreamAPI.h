#pragma once
#include "pch.h"
#include "Config.h"
#include <Overlay_types.h>
#include <eos-sdk\eos_sdk.h>

namespace ScreamAPI{

extern HMODULE thisDLL;
extern HMODULE originalDLL;
extern EOS_HPlatform hPlatform;

void init(HMODULE hModule);
void checkSdkVersion(const int32_t apiVersion, const int32_t maxVersion);
void destroy();

// Have to define template function in header (-_-)
template <typename RetType, typename... ArgTypes>
struct proxyTraits{
	using funcType = RetType(EOS_CALL*)(ArgTypes...);
};

// A function that returns a type-safe reference to the requested EOS SDK function
template <typename RetType, typename... ArgTypes>
static auto proxyFunction(RetType(EOS_CALL* proxy)(ArgTypes...), LPCSTR rawFunctionName){
	using funcType = typename proxyTraits<RetType, ArgTypes...>::funcType;

#ifdef _WIN64
	std::string functionName = rawFunctionName;
#else
	// Need to decorate function name according to __stdcall convention:
	// https://docs.microsoft.com/en-us/cpp/build/reference/decorated-names?view=vs-2019#FormatC
	std::stringstream ss;
	ss << "_" << rawFunctionName << "@" << 4 * sizeof...(ArgTypes);
	std::string functionName = ss.str();
#endif
	// Get C-style pointer to function
	auto funcPtr = GetProcAddress(originalDLL, functionName.c_str());
	if(funcPtr)
		Logger::debug("Successfully proxied function: %s", functionName.c_str());
	else
		Logger::error("Failed to proxy function: %s", functionName.c_str());

	// Return type-safe version of that function pointer
	return reinterpret_cast<funcType>(funcPtr);
}
};


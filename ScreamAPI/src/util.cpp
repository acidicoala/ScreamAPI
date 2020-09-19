#include "pch.h"
#include "util.h"
#include "eos-sdk/eos_auth.h"
#include "eos-sdk/eos_sessions.h"

namespace Util{

std::filesystem::path getDLLparentDir(HMODULE hModule){
	WCHAR modulePathBuffer[MAX_PATH];
	GetModuleFileName(hModule, modulePathBuffer, MAX_PATH);

	std::filesystem::path modulePath = modulePathBuffer;
	return modulePath.parent_path();
}

EOS_HPlatform getHPlatform(){
	return ScreamAPI::hPlatform;
}

EOS_HAuth getHAuth(){
	static auto result = EOS_Platform_GetAuthInterface(getHPlatform());
	return result;
}

EOS_HConnect getHConnect(){
	static auto result = EOS_Platform_GetConnectInterface(getHPlatform());
	return result;
}

EOS_HAchievements getHAchievements(){
	static auto result = EOS_Platform_GetAchievementsInterface(getHPlatform());
	return result;
}

EOS_HSessions getHSessions(){
	static auto result = EOS_Platform_GetSessionsInterface(getHPlatform());
	return result;
}

EOS_EpicAccountId getEpicAccountId(){
	static auto result = EOS_Auth_GetLoggedInAccountByIndex(getHAuth(), 0);
	return result;
}

EOS_ProductUserId getProductUserId(){
	static auto result = EOS_Connect_GetLoggedInUserByIndex(getHConnect(), 0);
	return result;
}

#if FALSE
EOS_ProductUserId getProductUserIdnew(){
	Logger::debug(__func__);
	static auto sessionHanle = getHSessions();

	EOS_Sessions_CopyActiveSessionHandleOptions CopyActiveSessionOptions ={
		// TODO
	};
	static EOS_HActiveSession activeSessionHandle;
	EOS_Sessions_CopyActiveSessionHandle(sessionHanle, &CopyActiveSessionOptions, &activeSessionHandle);

	EOS_ActiveSession_GetRegisteredPlayerByIndexOptions GetRegisteredPlayerOptions = {
		EOS_ACTIVESESSION_GETREGISTEREDPLAYERBYINDEX_API_LATEST,
		0
	};
	static auto result = EOS_ActiveSession_GetRegisteredPlayerByIndex(activeSessionHandle, &GetRegisteredPlayerOptions);
	return result;
}
#endif

/**
 * A small utility function that copies the c string into a newly allocated memory
 * @return Pointer to the new string
 */
char* copy_c_string(const char* c_string){
	// Get string size
	auto string_size = strlen(c_string) + 1;// +1 for null terminator

	// Allocate enough memory for the new string
	char* new_string = new char[string_size];

	// Copy the string contents
	strcpy_s(new_string, string_size, c_string);

	return new_string;
}

}

#include "pch.h"
#include "ScreamAPI.h"

// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch(ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule); // Do not disturb us with thread calls
			ScreamAPI::init(hModule);
			break;
		case DLL_PROCESS_DETACH:
			ScreamAPI::destroy();
			break;
	}
	return TRUE;
}

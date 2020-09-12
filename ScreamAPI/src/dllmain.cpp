#include "pch.h"
#include "ScreamAPI.h"

// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain(HINSTANCE  hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch(ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			// Do not disturb us with thread calls
			DisableThreadLibraryCalls(hModule); // Fixme: Does not actually work.
			ScreamAPI::init(hModule);
			break;
		case DLL_PROCESS_DETACH:
			ScreamAPI::destroy();
			break;
	}
	return TRUE;
}

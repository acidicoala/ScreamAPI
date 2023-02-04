#include "scream_api/scream_api.hpp"

// This header will be populated at build time
#include <linker_exports.h>

#include <legacy_linker_exports.h>

EXTERN_C BOOL WINAPI DllMain(HMODULE handle, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        scream_api::init(handle);
    } else if (reason == DLL_PROCESS_DETACH) {
        // This isn't actually called on process exit
        scream_api::shutdown();
    }

    return TRUE;
}

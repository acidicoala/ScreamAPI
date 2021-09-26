#include "scream_api/scream_api.hpp"

// This header will be populated at build time
#include <linker_exports.h>

EXTERN_C BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            scream_api::init();
            break;
        case DLL_PROCESS_DETACH:
            scream_api::shutdown();
            break;
        default:
            break;
    }

    return TRUE;
}
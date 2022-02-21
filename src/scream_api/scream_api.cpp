#include "scream_api.hpp"
#include <build_config.h>

#include <sdk/eos_ecom.h>
#include <sdk/eos_metrics.h>
#include <sdk/eos_logging.h>
#include <sdk/eos_init.h>

#include "polyhook2/PE/IatHook.hpp"

#define HOOK(METHOD, FUNC) \
    hook::METHOD( \
        original_library, \
        loader::get_undecorated_function(original_library, #FUNC), \
        reinterpret_cast<FunctionPointer>(FUNC) \
    );

#define DETOUR(FUNC) HOOK(detour_or_throw, FUNC)
#define EAT_HOOK(FUNC) HOOK(eat_hook_or_throw, FUNC)

namespace scream_api {
    Config config = {};

    String namespace_id;

    HMODULE original_library = nullptr;

    bool is_hook_mode = false;

    void init(HMODULE self_module) {
        DisableThreadLibraryCalls(self_module);

        const auto self_directory = loader::get_module_dir(self_module);

        config = config_parser::parse<Config>(self_directory / PROJECT_NAME".json");

        if (config.logging) {
            logger = file_logger::create(self_directory / PROJECT_NAME".log");
        }

        logger->info("🐨 {} v{}", PROJECT_NAME, PROJECT_VERSION);

        is_hook_mode = hook::is_hook_mode(self_module, ORIGINAL_DLL);

        if (is_hook_mode) {
            logger->info("🪝 Detected hook mode"); // New hook emoji:

            dll_monitor::init(ORIGINAL_DLL, [](const HMODULE& library) {
                original_library = library;

                hook::init([]() {

                    DETOUR(EOS_Ecom_CopyEntitlementByIndex)
                    DETOUR(EOS_Ecom_CopyEntitlementByNameAndIndex)
                    DETOUR(EOS_Ecom_CopyItemById)
                    DETOUR(EOS_Ecom_Entitlement_Release)
                    DETOUR(EOS_Ecom_GetEntitlementsByNameCount)
                    DETOUR(EOS_Ecom_GetEntitlementsCount)
                    DETOUR(EOS_Ecom_GetItemReleaseCount)
                    DETOUR(EOS_Ecom_QueryEntitlements)
                    DETOUR(EOS_Ecom_QueryOwnership)
                    DETOUR(EOS_Ecom_QueryOwnershipToken)
                    DETOUR(EOS_Ecom_RedeemEntitlements)
                    DETOUR(EOS_Initialize)
                    DETOUR(EOS_Platform_Create)
                    DETOUR(EOS_Metrics_BeginPlayerSession)
                    DETOUR(EOS_Metrics_EndPlayerSession)

                    if (config.logging && config.eos_logging) {
                        // Function prologue is too small to hook via detour
                        EAT_HOOK(EOS_Logging_SetCallback)
                        DETOUR(EOS_Logging_SetLogLevel)
                    }
                });
            });

        } else {
            logger->info("🔀 Detected proxy mode");

            original_library = loader::load_original_library(self_directory, ORIGINAL_DLL);
        }

        logger->info("🚀 Initialization complete");
    }

    void shutdown() {
        if (is_hook_mode) {
            dll_monitor::shutdown();
        } else {
            win_util::free_library(original_library);
        }

        logger->info("💀 Shutdown complete");
    }

}

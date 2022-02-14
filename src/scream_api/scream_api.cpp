#include "scream_api.hpp"
#include "config/config.hpp"
#include <build_config.h>

#include <sdk/eos_ecom.h>
#include <sdk/eos_metrics.h>
#include <sdk/eos_logging.h>
#include <sdk/eos_init.h>

#include "koalabox/dll_monitor/dll_monitor.hpp"
#include "koalabox/hook/hook.hpp"
#include "koalabox/loader/loader.hpp"
#include "koalabox/logger/logger.hpp"
#include "koalabox/win_util/win_util.hpp"

namespace scream_api {
    using namespace koalabox;

    HMODULE original_library = nullptr;

    bool is_hook_mode = false;

    void init(HMODULE self_module) {
        DisableThreadLibraryCalls(self_module);

        const auto self_directory = loader::get_module_dir(self_module);

        config::init(self_directory / PROJECT_NAME".json");

        if (config::instance.logging) {
            logger::init(self_directory / PROJECT_NAME".log");
        }

        logger::info("🐨 {} v{}", PROJECT_NAME, PROJECT_VERSION);

        is_hook_mode = hook::is_hook_mode(self_module, ORIG_DLL);

        if (is_hook_mode) {
            logger::info("Detected hook mode");

            dll_monitor::init(ORIG_DLL, [](HMODULE library) {
                original_library = library;

                hook::init([&]() {

#define HOOK(FUNC)  hook::detour_with_fallback( \
                        library, \
                        loader::get_undecorated_function(library, #FUNC), \
                        reinterpret_cast<hook::FunctionPointer>(FUNC) \
                    );

                    HOOK(EOS_Ecom_CopyEntitlementByIndex)
                    HOOK(EOS_Ecom_CopyEntitlementByNameAndIndex)
                    HOOK(EOS_Ecom_CopyItemById)
                    HOOK(EOS_Ecom_Entitlement_Release)
                    HOOK(EOS_Ecom_GetEntitlementsByNameCount)
                    HOOK(EOS_Ecom_GetEntitlementsCount)
                    HOOK(EOS_Ecom_GetItemReleaseCount)
                    HOOK(EOS_Ecom_QueryEntitlements)
                    HOOK(EOS_Ecom_QueryOwnership)
                    HOOK(EOS_Ecom_QueryOwnershipToken)
                    HOOK(EOS_Ecom_RedeemEntitlements)
                    HOOK(EOS_Initialize)
                    HOOK(EOS_Platform_Create)
                    HOOK(EOS_Metrics_BeginPlayerSession)
                    HOOK(EOS_Metrics_EndPlayerSession)

                    if (config::instance.logging && config::instance.eos_logging) {
                        HOOK(EOS_Logging_SetCallback)
                        HOOK(EOS_Logging_SetLogLevel)
                    }
                });
            });

        } else {
            logger::info("Detected proxy mode");

            original_library = loader::load_original_library(self_directory, ORIG_DLL);
        }

        logger::info("🚀 Initialization complete");
    }

    void shutdown() {
        if (is_hook_mode) {
            dll_monitor::shutdown();
        } else {
            win_util::free_library(original_library);
        }

        logger::info("💀 Shutdown complete");
    }

}

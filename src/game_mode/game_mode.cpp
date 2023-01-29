#include <game_mode/game_mode.hpp>
#include <scream_api/config.hpp>
#include <build_config.h>

#include <koalabox/dll_monitor.hpp>
#include <koalabox/hook.hpp>
#include <koalabox/loader.hpp>
#include <koalabox/logger.hpp>
#include <koalabox/paths.hpp>

#include <sdk/eos_ecom.h>
#include <sdk/eos_init.h>
#include <sdk/eos_logging.h>
#include <sdk/eos_metrics.h>

#define HOOK(FUNC) \
    koalabox::hook::detour_or_warn( \
        scream_api::game_mode::original_library, \
        koalabox::loader::get_decorated_function(scream_api::game_mode::original_library, #FUNC), \
        reinterpret_cast<uintptr_t>(FUNC) \
    );

namespace scream_api::game_mode {

    String namespace_id;

    HMODULE original_library = nullptr;

    bool is_hook_mode = false;

    void init_hook_mode() {
        koalabox::hook::init(true);

        LOG_INFO("🪝 Detected hook mode")

        koalabox::dll_monitor::init_listener(EOSSDK_DLL, [](const HMODULE& library) {
            original_library = library;

            koalabox::hook::init();

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

            if (config::instance.block_metrics) {
                HOOK(EOS_Metrics_BeginPlayerSession)
                HOOK(EOS_Metrics_EndPlayerSession)
            }

            if (config::instance.logging && config::instance.eos_logging) {
                HOOK(EOS_Logging_SetCallback)
                HOOK(EOS_Logging_SetLogLevel)
            }

            // Note: We should not shut down listener after the first callback
            // because some games may mistakenly load EOS SDK dll more than once.
        });
    }

    void init_proxy_mode() {
        LOG_INFO("🔀 Detected proxy mode")

        const auto self_path = koalabox::paths::get_self_path();
        original_library = koalabox::loader::load_original_library(self_path, EOSSDK_DLL);
    }

    void init_game_mode(HMODULE self_handle) {
        LOG_INFO("🎮 Detected game mode")

        is_hook_mode = koalabox::hook::is_hook_mode(self_handle, EOSSDK_DLL);

        if (is_hook_mode) {
            init_hook_mode();
        } else {
            init_proxy_mode();
        }
    }

}

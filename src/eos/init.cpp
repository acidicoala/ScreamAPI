#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/logger.hpp>

#include <sdk/eos_init.h>
#include <sdk/eos_logging.h>

DLL_EXPORT(EOS_EResult) EOS_Initialize(const EOS_InitializeOptions* Options) {
    GET_ORIGINAL_FUNCTION(EOS_Initialize)

    if (Options) {
        LOG_DEBUG("EOS_Initialize -> ProductName: '{}'", Options->ProductName)
    }

    auto result = EOS_Initialize_o(Options);

    if (CONFIG.logging && CONFIG.eos_logging) {
        EOS_Logging_SetLogLevel(
            EOS_ELogCategory::EOS_LC_ALL_CATEGORIES,
            EOS_ELogLevel::EOS_LOG_VeryVerbose
        );

        EOS_Logging_SetCallback(
            [](const EOS_LogMessage* Message) {
                LOG_DEBUG("[{}]\t{}", Message->Category, Message->Message)
            }
        );
    }

    return result;
}

DLL_EXPORT(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options) {
    GET_ORIGINAL_FUNCTION(EOS_Platform_Create)

    if (Options) {
        LOG_INFO("🗃️ DLC database: https://scream-db.web.app/offers/{}", Options->SandboxId)

        scream_api::game_mode::namespace_id = Options->SandboxId;
    }

    return EOS_Platform_Create_o(Options);
}

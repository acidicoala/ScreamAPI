#include <sdk/eos_init.h>
#include <sdk/eos_logging.h>
#include "config/config.hpp"
#include "koalabox/logger/logger.hpp"
#include "scream_api/scream_api.hpp"

using namespace koalabox;

std::string scream_api::namespace_id;

DLL_EXPORT(EOS_EResult) EOS_Initialize(const EOS_InitializeOptions* Options) {
    GET_ORIGINAL_FUNCTION(EOS_Initialize)

    if (Options) {
        logger::debug("EOS_Initialize -> ProductName: '{}'", Options->ProductName);
    }

    auto result = EOS_Initialize_o(Options);

    if (config::instance.logging && config::instance.eos_logging) {
        EOS_Logging_SetLogLevel(
            EOS_ELogCategory::EOS_LC_ALL_CATEGORIES,
            EOS_ELogLevel::EOS_LOG_VeryVerbose
        );

        EOS_Logging_SetCallback(
            [](const EOS_LogMessage* Message) {
                logger::debug("[{}]\t{}", Message->Category, Message->Message);
            }
        );
    }

    return result;
}

DLL_EXPORT(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options) {
    GET_ORIGINAL_FUNCTION(EOS_Platform_Create)

    if (Options) {
        logger::info("🗃 DLC database: https://scream-db.web.app/offers/{}", Options->SandboxId);
        scream_api::namespace_id = Options->SandboxId;
    }

    return EOS_Platform_Create_o(Options);
}

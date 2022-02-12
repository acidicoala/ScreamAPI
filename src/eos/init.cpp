#include <sdk/eos_init.h>
#include <sdk/eos_logging.h>
#include "config/config.hpp"
#include "koalabox/logger/logger.hpp"
#include "scream_api/scream_api.hpp"

using namespace koalabox;

std::string scream_api::namespace_id;

DLL_EXPORT(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options) {
    static auto EOS_Platform_Create_o = scream_api::get_original_function(
        &EOS_Platform_Create,
        __func__
    );

    if (Options != nullptr) {
        logger::info("🗃 DLC database: https://scream-db.web.app/offers/{}", Options->SandboxId);
        scream_api::namespace_id = Options->SandboxId;
    }

    auto result = EOS_Platform_Create_o(Options);

    if (config::instance.logging) {
        // Enable EOS logging
        EOS_Logging_SetCallback(
            [](const EOS_LogMessage* Message) {
                logger::debug("[{}]\t{}", Message->Category, Message->Message);
            }
        );

        EOS_Logging_SetLogLevel(
            EOS_ELogCategory::EOS_LC_ALL_CATEGORIES,
            EOS_ELogLevel::EOS_LOG_VeryVerbose
        );
    }

    return result;
}

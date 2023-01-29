#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/logger.hpp>

#include <sdk/eos_ecom.h>

using namespace scream_api;

DLL_EXPORT(void) EOS_Ecom_QueryOwnership(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryOwnershipOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate
) {
    GET_ORIGINAL_FUNCTION(EOS_Ecom_QueryOwnership)

    LOG_INFO("❓ Game requested ownership of {} items:", Options->CatalogItemIdCount)
    for (uint32_t i = 0; i < Options->CatalogItemIdCount; i++) {
        LOG_INFO("  ❔ {}", Options->CatalogItemIds[i])
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate;
    };

    EOS_Ecom_QueryOwnership_o(Handle, Options, new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryOwnershipCallbackInfo* Data) {
            const auto container = static_cast<Container*>(Data->ClientData);

            LOG_INFO("🍀 ScreamAPI prepared {} items:", Data->ItemOwnershipCount)

            for (uint32_t i = 0; i < Data->ItemOwnershipCount; i++) {
                const auto item = Data->ItemOwnership + i;

                const auto unlock_all = scream_api::config::instance.catalog_items.unlock_all;
                const auto override = scream_api::config::instance.catalog_items.override.contains(item->Id);
                const auto owned = unlock_all != override;  // Logical XOR

                const auto status = owned
                    ? EOS_EOwnershipStatus::EOS_OS_Owned
                    : EOS_EOwnershipStatus::EOS_OS_NotOwned;

                const auto ownership_icon = owned
                    ? "✅"
                    : "❌";

                const auto ownership_status = owned
                    ? "Owned"
                    : "Not Owned";

                const_cast<EOS_Ecom_ItemOwnership*>(item)->OwnershipStatus = status;

                LOG_INFO("  {} {} ({})", ownership_icon, item->Id, ownership_status)
            }

            const_cast<EOS_Ecom_QueryOwnershipCallbackInfo*>(Data)->ClientData = container->ClientData;

            container->CompletionDelegate(Data);

            delete container;
        }
    );
}

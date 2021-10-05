#include <sdk/eos_ecom.h>
#include <sdk/eos_common.h>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "scream_api/scream_api.hpp"
#include "std_ext.hpp"

#include <chrono>

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryOwnershipOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate
) {
    static auto proxy = scream_api::get_original_function(&EOS_Ecom_QueryOwnership, __func__);

    logger::info("❓ Game requested ownership of {} items:", Options->CatalogItemIdCount);
    for (uint32_t i = 0; i < Options->CatalogItemIdCount; i++) {
        auto id = Options->CatalogItemIds[i];
        logger::info("  ❔ {}", id);
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate;
    };

    proxy(Handle, Options, new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryOwnershipCallbackInfo* Data) {
            const auto container = static_cast<Container*>(Data->ClientData);

            logger::info("🍀 ScreamAPI prepared {} items:", Data->ItemOwnershipCount);

            for (uint32_t i = 0; i < Data->ItemOwnershipCount; i++) {
                auto item = Data->ItemOwnership + i;

                const auto unlock_all = config::get().catalog_items.unlock_all;
                const auto override = config::get().catalog_items.override.contain(item->Id);
                const auto owned = unlock_all != override;  // Logical XOR

                const auto status = owned
                    ? EOS_EOwnershipStatus::EOS_OS_Owned
                    : EOS_EOwnershipStatus::EOS_OS_NotOwned;

                const auto ownership_icon = owned
                    ? "✔"
                    : "❌";

                const auto ownership_status = owned
                    ? "Owned"
                    : "Not Owned";

                const_cast<EOS_Ecom_ItemOwnership*>(item)->OwnershipStatus = status;

                logger::info("  {} {} ({})", ownership_icon, item->Id, ownership_status);
            }

            const_cast<EOS_Ecom_QueryOwnershipCallbackInfo*>(Data)->ClientData = container->ClientData;

            container->CompletionDelegate(Data);

            delete container;
        }
    );
}

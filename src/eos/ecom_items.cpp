#include <sdk/eos_ecom.h>
#include <sdk/eos_common.h>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "std_ext.hpp"

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(
    EOS_HEcom,
    const EOS_Ecom_QueryOwnershipOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate
) {
    static std_ext::list<EOS_Ecom_ItemOwnership> items;

    auto add_without_duplicates = [](const char* item_id, const EOS_EOwnershipStatus status) {
        auto item_id_matches = [&](const EOS_Ecom_ItemOwnership& item) {
            return strcmp(item_id, item.Id) == 0;
        };

        if (not items.contain(item_id_matches)) {
            items.add(
                EOS_Ecom_ItemOwnership{
                    .ApiVersion = EOS_ECOM_ITEMOWNERSHIP_API_LATEST,
                    .Id = _strdup(item_id),
                    .OwnershipStatus = status,
                }
            );
        }
    };

    // log item IDs
    logger::info("❓ Game requested ownership of {} items:", Options->CatalogItemIdCount);
    for (uint32_t i = 0; i < Options->CatalogItemIdCount; i++) {
        auto id = Options->CatalogItemIds[i];
        logger::info("  ❔ {}", id);

        auto unlock_all = config::get().catalog_items.unlock_all;
        auto override = config::get().catalog_items.override.contain(id);
        auto owned = unlock_all != override;  // Logical XOR

        auto status = owned
            ? EOS_EOwnershipStatus::EOS_OS_Owned
            : EOS_EOwnershipStatus::EOS_OS_NotOwned;

        add_without_duplicates(id, status);
    }

    char local_user_id[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
    int32_t buffer_size = sizeof(local_user_id);

    EOS_EpicAccountId_ToString(Options->LocalUserId, local_user_id, &buffer_size);

    // Execute blocking operation in a new thread to immediately return control to the game
    std::thread(
        [=]() { // Capture by value original parameters
            logger::info("🍀 ScreamAPI prepared {} items:", items.count());

            for (const auto& item: items.vector) {
                auto ownership_icon = item.OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned
                    ? "✔"
                    : "❌";

                auto ownership_status = item.OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned
                    ? "Owned"
                    : "Not Owned";

                logger::info("  {} {} ({})", ownership_icon, item.Id, ownership_status);
            }

            const EOS_Ecom_QueryOwnershipCallbackInfo data{
                .ResultCode = EOS_EResult::EOS_Success,
                .ClientData = ClientData,
                .LocalUserId = EOS_EpicAccountId_FromString(local_user_id),
                .ItemOwnership = items.raw(),
                .ItemOwnershipCount = items.count(),
            };
            CompletionDelegate(&data);
        }
    ).detach();
}

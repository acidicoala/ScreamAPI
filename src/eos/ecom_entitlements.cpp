#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <scream_api/api.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/http_client.hpp>
#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>

#include <sdk/eos_ecom.h>

using namespace scream_api;

Vector<String> entitlements; // ids
Map<String, String> entitlement_map;  // id => title

// This hook does not perform any unlocking yet.
// It is implemented to collect debug logs for potential improvements.
DLL_EXPORT(void) EOS_Ecom_QueryEntitlementToken(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryEntitlementTokenOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryEntitlementTokenCallback CompletionDelegate // NOLINT(misc-misplaced-const)
) {
    GET_ORIGINAL_FUNCTION(EOS_Ecom_QueryEntitlementToken)

    const auto orig = [&]() {
        EOS_Ecom_QueryEntitlementToken_o(Handle, Options, ClientData, CompletionDelegate);
    };

    if (not Options) {
        return orig();
    }

    LOG_DEBUG("{} -> EntitlementNameCount: {}", __func__, Options->EntitlementNameCount)

    const auto names = Options->EntitlementNames;

    if (not names) {
        return orig();
    }

    for (uint32_t i = 0; i < Options->EntitlementNameCount; i++) {
        const auto name = names[i];

        if (name) {
            LOG_DEBUG("  EntitlementName: {}", name)
        }
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryEntitlementTokenCallback CompletionDelegate;
    };

    EOS_Ecom_QueryEntitlementToken_o(
        Handle, Options,
        new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryEntitlementTokenCallbackInfo* Data) {
            const auto container = static_cast<Container*>(Data->ClientData);

            LOG_DEBUG(
                "EOS_Ecom_QueryEntitlementToken callback result : {}, token: {}",
                (uint32_t) Data->ResultCode, Data->EntitlementToken
            )

            auto* mData = const_cast<EOS_Ecom_QueryEntitlementTokenCallbackInfo*>(Data);

            mData->ResultCode = EOS_EResult::EOS_Success;
            mData->ClientData = container->ClientData;

            container->CompletionDelegate(Data);

            delete container;
        }
    );
}

DLL_EXPORT(void) EOS_Ecom_QueryEntitlements(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryEntitlementsOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate // NOLINT(misc-misplaced-const)
) {
    GET_ORIGINAL_FUNCTION(EOS_Ecom_QueryEntitlements)

    LOG_INFO("❓ Game requested {} entitlements:", Options->EntitlementNameCount)

    entitlement_map.clear();

    for (uint32_t i = 0; i < Options->EntitlementNameCount; i++) {
        const auto id = Options->EntitlementNames[i];
        LOG_INFO("  ❔ {}", id)

        const auto owned = scream_api::config::is_dlc_unlocked(
            scream_api::game_mode::namespace_id,
            id,
            true
        );

        if (owned) {
            entitlement_map[id] = fmt::format("DLC {}", id);
        }
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate;
    };

    EOS_Ecom_QueryEntitlements_o(
        Handle, Options,
        new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryEntitlementsCallbackInfo* Data) {
            try {
                const auto container = static_cast<Container*>(Data->ClientData);

                const auto& namespace_id = scream_api::game_mode::namespace_id;

                // Inject online entitlements
                if (namespace_id.empty()) {
                    LOG_WARN(
                        "EOS_Ecom_QueryEntitlements callback: namespace_id is not set. "
                        "Perhaps you should try another loader DLL, "
                        "so that ScreamAPI is loaded before EOS SDK dll"
                    )
                } else if (entitlement_map.empty()) {
                    const auto entitlements_opt = api::fetch_entitlements(namespace_id);

                    if (entitlements_opt) {
                        for (const auto& [id, title]: *entitlements_opt) {
                            const auto is_unlocked = scream_api::config::is_dlc_unlocked(
                                namespace_id,
                                id,
                                false
                            );

                            if (is_unlocked) {
                                LOG_DEBUG(
                                    R"(➕ Adding auto-fetched entitlement: {} - "{}")", id, title
                                )
                                entitlement_map[id] = title;
                            }
                        }
                    }
                }

                // Inject extra entitlements
                const auto& extra = CONFIG.extra_entitlements;
                if (extra.contains(namespace_id)) {
                    const auto& game = extra.at(namespace_id);

                    for (auto& [id, title]: game.entitlements) {
                        const auto is_unlocked = scream_api::config::is_dlc_unlocked(
                            namespace_id,
                            id,
                            true
                        );

                        if (is_unlocked) {
                            LOG_DEBUG(R"(Adding entitlement from config: {} - "{}")", id, title)

                            entitlement_map[id] = title;
                        }
                    }
                }

                entitlements.clear();

                auto ids = std::views::keys(entitlement_map);
                entitlements = { ids.begin(), ids.end() };

                LOG_INFO("🍀 ScreamAPI prepared {} entitlements:", entitlements.size())
                for (const auto& id: entitlements) {
                    LOG_INFO(R"(  ✅ {} = "{}")", id, entitlement_map[id])
                }

                auto* mData = const_cast<EOS_Ecom_QueryEntitlementsCallbackInfo*>(Data);

                mData->ResultCode = EOS_EResult::EOS_Success;
                mData->ClientData = container->ClientData;

                container->CompletionDelegate(Data);

                delete container;
            } catch (const Exception& e) {
                LOG_ERROR("EOS_Ecom_QueryEntitlements callback error: {}", e.what())
            }
        }
    );
}

DLL_EXPORT(uint32_t) EOS_Ecom_GetEntitlementsCount(
    EOS_HEcom,
    const EOS_Ecom_GetEntitlementsCountOptions*
) {
    const auto count = entitlements.size();

    LOG_DEBUG("Responding with the count of {} entitlements", count)

    return count;
}

DLL_EXPORT(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(
    EOS_HEcom,
    const EOS_Ecom_CopyEntitlementByIndexOptions* Options,
    EOS_Ecom_Entitlement** OutEntitlement
) {
    const auto index = Options->EntitlementIndex;
    if (index >= entitlements.size()) {
        LOG_WARN(
            "Game requested invalid entitlement index: {}. Max size: {}",
            index, entitlements.size()
        )

        return EOS_EResult::EOS_NotFound;
    }

    const auto id = entitlements[index].c_str();

    LOG_DEBUG("Copying the entitlement: {} at index: {}", id, index)

    *OutEntitlement = new EOS_Ecom_Entitlement{
        .ApiVersion = EOS_ECOM_ENTITLEMENT_API_LATEST,
        .EntitlementName = id,
        .EntitlementId = id,
        .CatalogItemId = id,
        .ServerIndex = -1,
        .bRedeemed = false,
        .EndTimestamp = -1,
    };

    return EOS_EResult::EOS_Success;
}

DLL_EXPORT(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement) {
    if (Entitlement) {
        LOG_DEBUG("Freeing a copy of the entitlement: {}", Entitlement->EntitlementName)
        delete Entitlement;
    } else {
        LOG_WARN("Game attempted to free a null entitlement")
    }
}

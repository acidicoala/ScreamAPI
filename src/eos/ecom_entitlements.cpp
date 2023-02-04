#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/http_client.hpp>
#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>

#include <sdk/eos_ecom.h>

using namespace scream_api;

static Vector<String> entitlements;

DLL_EXPORT(void) EOS_Ecom_QueryEntitlements(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryEntitlementsOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate
) {
    GET_ORIGINAL_FUNCTION(EOS_Ecom_QueryEntitlements)

    static Set<String> entitlement_set;

    LOG_INFO("❓ Game requested {} entitlements:", Options->EntitlementNameCount)

    for (uint32_t i = 0; i < Options->EntitlementNameCount; i++) {
        const auto id = Options->EntitlementNames[i];
        LOG_INFO("  ❔ {}", id)

        if (scream_api::config::instance.entitlements.unlock_all) {
            entitlement_set.insert(id);
        }
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate;
    };

    EOS_Ecom_QueryEntitlements_o(Handle, Options, new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryEntitlementsCallbackInfo* Data) {
            try {
                const auto container = static_cast<Container*>(Data->ClientData);

                // Manually inject entitlements
                for (auto& id: scream_api::config::instance.entitlements.inject) {
                    LOG_DEBUG("Adding entitlement from config: {}", id)
                    entitlement_set.insert(id);
                }

                // Automatically inject entitlements
                if (scream_api::game_mode::namespace_id.empty()) {
                    LOG_WARN("EOS_Ecom_QueryEntitlements callback: namespace_id is not set")
                } else {
                    if (scream_api::config::instance.entitlements.auto_inject) {
                        nlohmann::json payload = {
                            { "query",     R"(query($namespace: String!) {
                        Catalog {
                            catalogOffers(
                                namespace: $namespace
                                params: {
                                    count: 1000,
                                }
                            ) {
                                elements {
                                    items {
                                        id
                                    }
                                }
                            }
                        }
                    })" },
                            { "variables", {{ "namespace", scream_api::game_mode::namespace_id }}}
                        };

                        try {
                            const auto json = koalabox::http_client::post_json(
                                "https://graphql.epicgames.com/graphql",
                                payload
                            );

                            LOG_DEBUG("Response json:\n{}", json.dump(2))

                            const auto elements = json["data"]["Catalog"]["catalogOffers"]["elements"];

                            for (const auto& element: elements) {
                                for (const auto& items: element) {
                                    for (const auto& item: items) {
                                        String id(item["id"]);
                                        LOG_DEBUG("Adding auto-fetched entitlement: {}", id)
                                        entitlement_set.insert(id);
                                    }
                                }
                            }
                        } catch (const Exception& e) {
                            LOG_ERROR("Error fetching entitlement ids: {}", e.what())
                        }
                    }
                }

                entitlements = std::vector(entitlement_set.begin(), entitlement_set.end());

                LOG_INFO("🍀 ScreamAPI prepared {} entitlements:", entitlements.size())
                for (const auto& entitlement: entitlements) {
                    LOG_INFO("  ✅ {}", entitlement)
                }

                const_cast<EOS_Ecom_QueryEntitlementsCallbackInfo*>(Data)->ResultCode = EOS_EResult::EOS_Success;
                const_cast<EOS_Ecom_QueryEntitlementsCallbackInfo*>(Data)->ClientData = container->ClientData;

                container->CompletionDelegate(Data);

                delete container;
            } catch (const Exception& ex) {
                koalabox::util::panic("EOS_Ecom_QueryEntitlements callback error: {}", ex.what());
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
        LOG_WARN("Game requested invalid entitlement index: {}. Max size: {}", index, entitlements.size())

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
        .bRedeemed = true, // TODO: Make it configurable?
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

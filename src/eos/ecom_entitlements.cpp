#include <sdk/eos_ecom.h>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "util/util.hpp"
#include "scream_api/scream_api.hpp"
#include "std_ext.hpp"

static std_ext::list<EOS_Ecom_Entitlement> entitlements;

void add_without_duplicates(const char* entitlement_id, EOS_Bool redeemed = EOS_TRUE) {
    auto entitlement_id_matches = [&](const EOS_Ecom_Entitlement& entitlement) {
        return strcmp(entitlement_id, entitlement.EntitlementName) == 0;
    };

    if (not entitlements.contain(entitlement_id_matches)) {
        auto id = _strdup(entitlement_id);
        entitlements.add(
            EOS_Ecom_Entitlement{
                .ApiVersion = EOS_ECOM_ENTITLEMENT_API_LATEST,
                .EntitlementName = id,
                .EntitlementId = id,
                .CatalogItemId = id,
                .ServerIndex = -1,
                .bRedeemed = redeemed,
                .EndTimestamp = -1,
            }
        );
    }
};

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlements(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryEntitlementsOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate
) {
    static auto proxy = scream_api::get_original_function(&EOS_Ecom_QueryEntitlements, __func__);

    logger::info("❓ Game requested {} entitlements:", Options->EntitlementNameCount);

    for (uint32_t i = 0; i < Options->EntitlementNameCount; i++) {
        auto id = Options->EntitlementNames[i];
        logger::info("  ❔ {}", id);

        if (config::get().entitlements.unlock_all) {
            logger::debug("Adding entitlement requested by game: {}", id);
            add_without_duplicates(id);
        }
    }

    struct Container {
        void* ClientData;
        EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate;
    };

    proxy(Handle, Options, new Container{ ClientData, CompletionDelegate },
        [](const EOS_Ecom_QueryEntitlementsCallbackInfo* Data) {
            const auto container = static_cast<Container*>(Data->ClientData);

            // Manually inject entitlements
            for (auto& id: config::get().entitlements.inject.vector) {
                logger::debug("Adding entitlement from config: {}", id);
                add_without_duplicates(id.c_str());
            }

            // Automatically inject entitlements
            if (config::get().entitlements.auto_inject) {
                nlohmann::json payload = {
                    { "query",     "query($namespace: String!) {\
                        Catalog {\
                            catalogOffers(\
                                namespace: $namespace\
                                params: {\
                                    count: 1000,\
                                }\
                            ) {\
                                elements {\
                                    items {\
                                        id\
                                    }\
                                }\
                            }\
                        }\
                    }" },
                    { "variables", {{ "namespace", scream_api::namespace_id }}}
                };

                auto res = cpr::Post(
                    cpr::Url{ "https://graphql.epicgames.com/graphql" },
                    cpr::Header{{ "content-type", "application/json" }},
                    cpr::Body{ payload.dump() }
                );

                if (res.status_code == cpr::status::HTTP_OK) {
                    auto json = nlohmann::json::parse(res.text);

                    auto elements = json["data"]["Catalog"]["catalogOffers"]["elements"];

                    for (auto& element: elements) {
                        for (auto& items: element) {
                            for (auto& item: items) {
                                std::string id(item["id"]);
                                logger::debug("Adding auto-fetched entitlement: {}", id);
                                add_without_duplicates(id.c_str());
                            }
                        }
                    }
                } else {
                    logger::error(
                        "Failed to automatically fetch entitlement ids. "
                        "Status code: {}. Text: {}", res.status_code, res.text
                    );
                }
            }

            logger::info("🍀 ScreamAPI prepared {} entitlements:", entitlements.count());
            for (auto& entitlement: entitlements.vector) {
                auto redeemed = entitlement.bRedeemed == EOS_TRUE
                    ? "Redeemed"
                    : "Not Redeemed";

                logger::info("  ✔ {} ({})", entitlement.EntitlementName, redeemed);
            }

            const_cast<EOS_Ecom_QueryEntitlementsCallbackInfo*>(Data)->ResultCode = EOS_EResult::EOS_Success;
            const_cast<EOS_Ecom_QueryEntitlementsCallbackInfo*>(Data)->ClientData = container->ClientData;

            container->CompletionDelegate(Data);

            delete container;
        }
    );
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsCount(
    EOS_HEcom,
    const EOS_Ecom_GetEntitlementsCountOptions*
) {
    const auto count = entitlements.count();

    logger::debug("Responding with the count of {} entitlements", count);

    return count;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(
    EOS_HEcom,
    const EOS_Ecom_CopyEntitlementByIndexOptions* Options,
    EOS_Ecom_Entitlement** OutEntitlement
) {
    const auto index = Options->EntitlementIndex;
    if (index < 0 or index >= entitlements.count()) {
        util::panic(
            "EOS_Ecom_CopyEntitlementByIndex",
            "Invalid index access: {}. Max size: {}"_format(index, entitlements.count())
        );
    }

    const auto& entitlement = entitlements[index];

    logger::debug("Copying the entitlement: {}", entitlement.EntitlementName);

    // A shallow copy makes it easy to delete
    *OutEntitlement = new EOS_Ecom_Entitlement(entitlement);

    return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement) {
    if (Entitlement) {
        logger::debug("Releasing the memory of the entitlement: {}", Entitlement->EntitlementName);
        delete Entitlement;
    } else {
        logger::warn("Game attempted to delete a null entitlement");
    }
}

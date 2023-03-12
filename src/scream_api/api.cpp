#include <scream_api/api.hpp>

#include <koalabox/logger.hpp>

namespace api {

    std::optional<Map<String, String>> fetch_entitlements(const String& namespace_id) {
        try {
            Json payload = {
                { "query",     R"(
                                query($namespace: String!) {
                                    Catalog {
                                        catalogOffers(
                                            namespace: $namespace
                                            params: { count: 1000 }
                                        ) {
                                            elements {
                                                items {
                                                    id
                                                    title
                                                }
                                            }
                                        }
                                    }
                                }
                            )" },
                { "variables", {{ "namespace", namespace_id }}}
            };

            const auto json = koalabox::http_client::post_json(
                "https://graphql.epicgames.com/graphql",
                payload
            );

            LOG_DEBUG("Response json:\n{}", json.dump(2))

            const auto elements = json["data"]["Catalog"]["catalogOffers"]["elements"];

            Map<String, String> entitlements;

            for (const auto& element: elements) {
                for (const auto& items: element) {
                    for (const auto& item: items) {
                        entitlements[item["id"]] = item["title"];
                    }
                }
            }

            return entitlements;
        } catch (const Exception& e) {
            LOG_ERROR("Error fetching entitlement ids: {}", e.what())

            return std::nullopt;
        }
    }

}
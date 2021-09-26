#include <build_config.h>
#include "config.hpp"
#include "util/util.hpp"

namespace config {
    void from_json(const nlohmann::json& json, CatalogItems& items) {
        json.at("unlock_all").get_to(items.unlock_all);
        json.at("override").get_to(items.override.vector);
    }

    void from_json(const nlohmann::json& json, Entitlements& entitlements) {
        json.at("unlock_all").get_to(entitlements.unlock_all);
        json.at("auto_inject").get_to(entitlements.auto_inject);
        json.at("inject").get_to(entitlements.inject.vector);
    }

    void from_json(const nlohmann::json& json, Config& config) {
        json.at("logging").get_to(config.logging);
        json.at("block_metrics").get_to(config.block_metrics);
        json.at("catalog_items").get_to(config.catalog_items);
        json.at("entitlements").get_to(config.entitlements);
    }
}

const config::Config& config::get() {

    static auto config = [] {
        Config config = {};

        const auto config_path = util::get_working_dir() / PROJECT_NAME".json";

        if (not exists(config_path))
            return config;

        try {
            std::ifstream ifs(config_path);
            nlohmann::json json;
            ifs >> json;

            config = json.get<Config>();
        } catch (const std::exception& ex) {
            util::error_box("config::get", "Failed to parse config file: {}"_format(ex.what()));
            exit(GetLastError());
        }

        return config;
    }();

    return config;
}

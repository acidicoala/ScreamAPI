#include <build_config.h>
#include "config.hpp"
#include "util/util.hpp"

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

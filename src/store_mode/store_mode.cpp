#include <store_mode/store_mode.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/http_server.hpp>

namespace store_mode {

    void init_store_mode() {
        LOG_INFO("🛍️ Detected store mode")

        // TODO: Implement
        koalabox::http_server::start("127.0.0.1", 8081, "api.epicgames.dev", {
            { "/", [](const httplib::Request& req, httplib::Response& res) {
                res.set_content(
                    R"({"message": "Hello World from C++"})",
                    koalabox::http_server::CONTENT_TYPE_JSON
                );
            }}
        });
    }

}

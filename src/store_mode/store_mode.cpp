#include <store_mode/store_mode.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/http_server.hpp>

namespace store_mode {

    const String egs_api_host = "api.epicgames.dev";
    const unsigned int egs_api_port = 443;

    const String local_host = "127.0.0.1";
    const String port_proxy_ip = "127.65.43.21"; // TODO: Parameterize

    const unsigned int local_port = 8081; // TODO: Parameterize

    void init_store_mode() {
        LOG_INFO("🛍️ Detected store mode")

        // TODO: Implement
        koalabox::http_server::start_proxy_server(
            egs_api_host,
            egs_api_port,
            local_host,
            local_port,
            port_proxy_ip,
            {
                { "/", [](const httplib::Request& req, httplib::Response& res) {
                    res.set_content(
                        R"({"message": "Hello World from C++"})",
                        koalabox::http_server::CONTENT_TYPE_JSON
                    );
                }}
            }
        );
    }

}

#include <store_mode/store_mode.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/http_server.hpp>

namespace store_mode {

    const String local_host = "127.0.0.1";
    const String server_ip = "127.65.43.21"; // TODO: Parameterize
    const String server_host = "api.epicgames.dev";
    const unsigned int server_port = 8081; // TODO: Parameterize

    void init_store_mode() {
        LOG_INFO("🛍️ Detected store mode")

        // TODO: Implement
        koalabox::http_server::start(
            local_host,
            server_ip,
            server_host,
            server_port,
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

    void shutdown() {
        koalabox::http_server::shutdown(server_host);
    }

}

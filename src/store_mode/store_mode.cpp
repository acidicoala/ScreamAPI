#include <store_mode/store_mode.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/http_server.hpp>

namespace store_mode {

    const String egs_api_host = "api.epicgames.dev";
    const unsigned int egs_api_port = 443;

    const String local_host = "127.0.0.1";
    const String port_proxy_ip = "127.65.43.21"; // TODO: Parameterize

    const unsigned int local_port = 8081; // TODO: Parameterize

    const String ecom_path = "screamapi_ecom";

    void sdk_index_handler(const httplib::Request&, httplib::Response& res) {
        try {
            const auto ecom_url = fmt::format("https://{}/{}", egs_api_host, ecom_path);

            auto json = Json::parse(res.body);
            json["services"]["EcommerceService"]["BaseUrl"] = ecom_url;
            // TODO: Save the original URL

            res.headers.erase("Content-Length");
            res.set_content(
                json.dump(),
                koalabox::http_server::CONTENT_TYPE_JSON
            );
        } catch (const Exception& e) {
            LOG_ERROR("Error processing api index: {}", e.what())
        }
    }

    void ecom_handler(const httplib::Request&, httplib::Response& res) {
        res.headers.erase("Content-Length");
        res.set_content(
            R"({"message": "Hello from ScreamAPI 🐨"})",
            koalabox::http_server::CONTENT_TYPE_JSON
        );
    }

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
                { "/sdk/v1/default",               sdk_index_handler },
                { fmt::format("/{}.*", ecom_path), ecom_handler }
            }
        );
    }

}

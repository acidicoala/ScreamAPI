#include <store_mode/store_mode.hpp>
#include <scream_api/config.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>
#include <koalabox/paths.hpp>
#include <koalabox/cache.hpp>
#include <koalabox/crypto.hpp>
#include <koalabox/http_client.hpp>

#include <fstream>

namespace store_mode {
    void setup_mitmproxy() {
        const auto version = "9.0.1";
        const auto file_name = fmt::format("mitmproxy-{}-windows.zip", version);
        const auto file_path = koalabox::paths::get_cache_dir() / file_name;
        const auto download_url = fmt::format(
            "https://snapshots.mitmproxy.org/{}/{}", version, file_name
        );

        try {
            const auto mitmproxy = koalabox::cache::read_from_cache("mitmproxy", {});

            const auto cached_md5 = mitmproxy.at("md5").get<String>();
            const auto cached_etag = mitmproxy.at("etag").get<String>();

            const auto actual_md5 = koalabox::crypto::calculate_md5(file_path);
            LOG_DEBUG("Mitmproxy md5 cached={}, actual={}", cached_md5, actual_md5)
            if (cached_md5 < not_equals > actual_md5) {
                throw Exception();
            }

            const auto actual_etag = koalabox::http_client::head_etag(download_url);
            LOG_DEBUG("Mitmproxy etag cached={}, actual={}", cached_etag, actual_etag)
            if (cached_etag < not_equals > actual_etag) {
                throw Exception();
            }

            LOG_DEBUG("Cached Mitmproxy is valid")
        } catch (const Exception& e) {
            const auto etag = koalabox::http_client::download_file(download_url, file_path);
            const auto md5 = koalabox::crypto::calculate_md5(file_path);

            koalabox::cache::save_to_cache("mitmproxy", {
                { "md5",  md5 },
                { "etag", etag },
            });

            LOG_DEBUG("Valid Mitmproxy is downloaded and cached")
        }

        // TODO: Unzip
    }

    void init_store_mode() {
        NEW_THREAD({
            try {
                LOG_INFO("🛍️ Detected store mode")

                setup_mitmproxy();

                // Verify installation:
                // mitmproxy --version

                // Call mitmproxy with script args

                // Group this and mitmproxy processes into a job to synchronize their lifetime
                // https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects
            } catch (const Exception& e) {
                LOG_ERROR("Store mode init error: {}", e.what())
            }
        })
    }

}

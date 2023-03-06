#include <store_mode/store_mode.hpp>
#include <scream_api/config.hpp>
#include <build_config.h>

#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>
#include <koalabox/paths.hpp>
#include <koalabox/cache.hpp>
#include <koalabox/io.hpp>
#include <koalabox/crypto.hpp>
#include <koalabox/http_client.hpp>

#include <fstream>

namespace store_mode {

    void setup_mitmproxy() {
        try {
            const String version = "9.0.1";
            const String cache_key = "mitmproxy";
            const auto exe_name = cache_key + ".exe";
            const auto exe_path = koalabox::paths::get_self_path() / exe_name;
            const String expected_exe_md5 = "40b4e945d63b858fc653ca0c0f3a4b98";
            const auto zip_name = fmt::format("mitmproxy-{}-windows.zip", version);
            const auto zip_path = koalabox::paths::get_cache_dir() / zip_name;
            const auto download_url = fmt::format(
                "https://snapshots.mitmproxy.org/{}/{}", version, zip_name
            );

            const auto is_mitmproxy_downloaded = [&]() {
                try {
                    const auto mitmproxy = koalabox::cache::read_from_cache(cache_key, {});

                    const auto cached_md5 = mitmproxy.at("md5").get<String>();
                    const auto cached_etag = mitmproxy.at("etag").get<String>();

                    const auto actual_md5 = koalabox::crypto::calculate_md5(zip_path);
                    LOG_DEBUG("Mitmproxy md5 cached={}, actual={}", cached_md5, actual_md5)
                    if (cached_md5 < not_equals > actual_md5) {
                        return false;
                    }

                    const auto actual_etag = koalabox::http_client::head_etag(download_url);
                    LOG_DEBUG("Mitmproxy etag cached={}, actual={}", cached_etag, actual_etag)
                    if (cached_etag < not_equals > actual_etag) {
                        return false;
                    }

                    LOG_DEBUG("Cached Mitmproxy is valid")
                    return true;
                } catch (const Exception& e) {
                    return false;
                }
            };

            const auto download_mitmproxy = [&]() {
                const auto etag = koalabox::http_client::download_file(download_url, zip_path);
                const auto md5 = koalabox::crypto::calculate_md5(zip_path);

                koalabox::cache::save_to_cache(cache_key, {
                    { "md5",  md5 },
                    { "etag", etag },
                });

                LOG_DEBUG("Valid Mitmproxy is downloaded and cached")
            };

            String exe_md5;
            const auto is_mitmproxy_valid = [&]() {
                if (not exists(exe_path)) {
                    return false;
                }

                exe_md5 = koalabox::crypto::calculate_md5(exe_path);

                return expected_exe_md5 < equals > exe_md5;
            };

            const auto unzip_mitmproxy = [&]() {
                if (koalabox::io::unzip_file(zip_path, exe_name, exe_path.parent_path())) {
                    return;
                }

                throw koalabox::util::exception("Mitmproxy unzipping error");
            };

            if (not is_mitmproxy_downloaded()) {
                download_mitmproxy();
            }

            if (not is_mitmproxy_valid()) {
                unzip_mitmproxy();
            }

            if (not is_mitmproxy_valid()) {
                throw koalabox::util::exception(
                    R"("Mitmproxy exe at "{}" is invalid. expected md5={}, actual={})",
                    exe_path.string(), expected_exe_md5, exe_md5
                );
            }

            // TODO: Verify installation:
            // mitmproxy --version


            LOG_INFO("Valid Mitmproxy is setup")
        } catch (const Exception& e) {
            throw koalabox::util::exception("Mitmproxy setup error: {}", e.what());
        }
    }

    void run_mitmproxy() {
        const String egs_addon_py((char*) EGS_ADDON_HEX);

        LOG_TRACE("EGS Addon script: \n{}", egs_addon_py)

        const auto addon_path = koalabox::paths::get_self_path() / "egs_addon.py";

        koalabox::io::write_file(addon_path, egs_addon_py);

        if (koalabox::io::read_file(addon_path) != egs_addon_py) {
            throw koalabox::util::exception("EGS addon script mismatch");
        }

        // TODO: Start-Process
    }

    void configure_proxy(){
        // TODO:
        //  * Setup proxy in registry (if admin, else message box)
        //  * Install mitmproxy cert in root store
    }

    void init_store_mode() {
        NEW_THREAD({
            try {
                LOG_INFO("🛍️ Detected store mode")

                setup_mitmproxy();
                run_mitmproxy();
                configure_proxy();

                // TODO: Group this and mitmproxy processes into a job to synchronize their lifetime
                // https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects
            } catch (const Exception& e) {
                LOG_ERROR("Store mode init error: {}", e.what())
            }
        })
    }

}

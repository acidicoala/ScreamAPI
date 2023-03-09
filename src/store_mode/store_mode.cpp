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
#include <koalabox/win_util.hpp>

#include <fstream>
#include <WinSock2.h>
#include <ShlObj_core.h>
#include <winhttp.h>
#include <commctrl.h>

#pragma comment(lib, "WinHttp.lib")
#pragma comment(lib, "comctl32.lib")

// https://learn.microsoft.com/en-us/answers/questions/1184393/when-including-comctl32-lib-causes-the-ordinal-345
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace store_mode {

    String dialog_title = "ScreamAPI [Store mode]";
    String localhost_ip = "127.0.0.1";

    Path setup_mitmproxy() {
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
                const auto mitmproxy = koalabox::cache::get(cache_key, {});

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

            koalabox::cache::put(cache_key, {
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
            throw KException(
                R"("Mitmproxy exe at "{}" is invalid. expected md5={}, actual={})",
                exe_path.string(), expected_exe_md5, exe_md5
            );
        }

        LOG_INFO("Valid Mitmproxy is setup")

        return exe_path;
    }

    void run_mitmproxy(const Path& exe_path) {
        const auto port = CONFIG.mitmproxy.listen_port;

        if (koalabox::io::is_local_port_in_use(port)) {
            const auto message = fmt::format(
                "Port {} is already in use. "
                "Perhaps another instance of mitmproxy is using it?", port
            );

            koalabox::util::error_box(dialog_title, message);

            throw Exception(message.c_str());
        }

        const String egs_addon_py((char*) EGS_ADDON_HEX);

        const auto addon_path = koalabox::paths::get_self_path() / "egs_addon.py";

        koalabox::io::write_file(addon_path, egs_addon_py);

        if (koalabox::io::read_file(addon_path) != egs_addon_py) {
            throw KException("EGS addon script mismatch");
        }

        const auto mitmproxy_info = koalabox::win_util::create_process(
            exe_path.string(),
            fmt::format("-s '{}' --listen-port {}", addon_path.string(), port),
            koalabox::paths::get_self_path(),
            CONFIG.mitmproxy.show_window
        );

        LOG_INFO("Launched mitmproxy at port {}", port)

        // Ensure that mitmproxy exits right after EGS process
        const auto job = CreateJobObject(NULL, NULL);
        if (job == NULL) {
            throw KException("Error creating job object: {}", GET_LAST_ERROR());
        }

        DECLARE_STRUCT(JOBOBJECT_EXTENDED_LIMIT_INFORMATION, job_info);
        job_info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

        if (!SetInformationJobObject(
            job,
            JobObjectExtendedLimitInformation,
            &job_info,
            sizeof(job_info)
        )) {
            throw KException("Error setting job object information: {}", GET_LAST_ERROR());
        }

        if (!AssignProcessToJobObject(job, mitmproxy_info.hProcess)) {
            throw KException("Error assigning process to job object: {}", GET_LAST_ERROR());
        }
    }

    void configure_proxy() {
        const auto install_certificate = []() {
            // TODO: Install mitmproxy cert in root store
        };

        const auto setup_system_proxy = []() {
            const auto port = std::to_string(CONFIG.mitmproxy.listen_port);
            std::wstring proxy = WSTR(fmt::format("{}:{}", localhost_ip, port));
            std::wstring bypass = L"<-loopback>;<local>";

            if (IsUserAnAdmin()) {
                WINHTTP_PROXY_INFO info = {
                    .dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY,
                    .lpszProxy = proxy.data(),
                    .lpszProxyBypass = bypass.data(),
                };

                if (WinHttpSetDefaultProxyConfiguration(&info) == FALSE) {
                    throw KException(
                        "WinHttpSetDefaultProxyConfiguration error: {}", GET_LAST_ERROR()
                    );
                }

                // TODO: Prompt to set always run as admin via registry key
            } else {
                const auto show_prompt_key = "show_proxy_config_prompt";
                const auto show_prompt = koalabox::cache::get(show_prompt_key, true);

                if (not show_prompt) {
                    return;
                }

                const auto header = L"Missing admin rights";
                const auto message = WSTR(fmt::format(
                    "ScreamAPI could not automatically configure system proxy "
                    "because the Epic Games Store is not running as administrator. "
                    "Please restart the Epic Games Store as administrator to enable "
                    "automatic proxy configuration or configure the system proxy manually "
                    "with the following parameters:\n\n"
                    "Address: {}\nPort: {}\nBypass: {}",
                    localhost_ip, port, STR(bypass)
                ));

                DECLARE_STRUCT(TASKDIALOGCONFIG, dialog_config);

                dialog_config.cbSize = sizeof(TASKDIALOGCONFIG);
                dialog_config.dwCommonButtons = TDCBF_CLOSE_BUTTON;
                dialog_config.pszWindowTitle = WSTR(dialog_title).c_str();
                dialog_config.pszMainIcon = TD_SHIELD_ICON;
                dialog_config.pszMainInstruction = header;
                dialog_config.pszContent = message.c_str();
                dialog_config.pszVerificationText = L"Do not show this message again.";
                dialog_config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;

                BOOL flag_checked = FALSE;
                if (SUCCEEDED(TaskDialogIndirect(
                    &dialog_config,
                    nullptr,
                    nullptr,
                    &flag_checked
                ))) {
                    if (flag_checked) {
                        koalabox::cache::put(show_prompt_key, false);
                    }
                }
            }
        };

        install_certificate();
        setup_system_proxy();
    }

    void init_store_mode() {
        NEW_THREAD({
            try {
                LOG_INFO("🛍️ Detected store mode")

                const auto exe_path = setup_mitmproxy();
                run_mitmproxy(exe_path);
                configure_proxy();
            } catch (const Exception& e) {
                LOG_ERROR("Store mode init error: {}", e.what())
            }
        })
    }

    void shutdown_store_mode() {
        LOG_DEBUG("Store mode shutting down")

        if (IsUserAnAdmin()) {
            WINHTTP_PROXY_INFO info = {
                .dwAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY,
                .lpszProxy = WINHTTP_NO_PROXY_NAME,
                .lpszProxyBypass = WINHTTP_NO_PROXY_BYPASS,
            };

            if (WinHttpSetDefaultProxyConfiguration(&info) == FALSE) {
                LOG_ERROR("WinHttpSetDefaultProxyConfiguration error: {}", GET_LAST_ERROR())
            }
        }
    }

}

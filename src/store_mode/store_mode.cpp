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

#include <WinReg.hpp>

#include <fstream>
#include <ShlObj_core.h>
#include <winhttp.h>
#include <commctrl.h>
#include <wincrypt.h>

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
        auto exe_path = koalabox::paths::get_self_path() / exe_name;
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

    void run_mitmproxy(const Path& mitmproxy_path) {
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
            mitmproxy_path.string(),
            fmt::format("-s '{}' --listen-port {}", addon_path.string(), port),
            koalabox::paths::get_self_path(),
            CONFIG.mitmproxy.show_window
        );

        LOG_INFO("Launched mitmproxy at port {}", port)

        // Ensure that mitmproxy exits right after EGS process
        const auto job = CreateJobObject(nullptr, nullptr);
        if (job == nullptr) {
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

    void configure_proxy(const Path& egl_path) {
        const auto install_certificate = []() {
            const auto user_dir = koalabox::paths::get_user_dir();
            const auto ca_path = user_dir / ".mitmproxy" / "mitmproxy-ca-cert.pem";
            const auto ca_string = koalabox::io::read_file(ca_path);
            LOG_TRACE("Adding cert to system store:\n{}", ca_string)

            Vector<uint8_t> binary_buffer(32 * 1024); // 32KB should be more than enough
            DWORD binary_size = binary_buffer.size();
            if (not CryptStringToBinary(
                WSTR(ca_string).c_str(),
                0,
                CRYPT_STRING_BASE64HEADER,
                binary_buffer.data(),
                &binary_size,
                nullptr,
                nullptr
            )) {
                throw KException("Error converting base64 certificate to binary", GET_LAST_ERROR());
            }

            // Will return true even if the cert is already installed
            if (not CertAddEncodedCertificateToSystemStore(
                TEXT("ROOT"),
                binary_buffer.data(),
                binary_size
            )) {
                throw KException(
                    "Error adding a certificate to the system 'ROOT' store: {}",
                    GET_LAST_ERROR()
                );
            }

            LOG_DEBUG("Mitmproxy certificate authority is present in the system 'ROOT' store")
        };

        const auto setup_system_proxy = [&]() {
            const auto port = std::to_string(CONFIG.mitmproxy.listen_port);
            std::wstring proxy = WSTR(fmt::format("{}:{}", localhost_ip, port));
            std::wstring bypass = L"<-loopback>;<local>";

            const auto show_task_dialog = [&](
                const String& cache_key,
                const String& header,
                const String& message,
                int common_buttons,
                const Function<void(int button)>& callback
            ) {
                const auto show_prompt = koalabox::cache::get(cache_key, true);

                if (not show_prompt) {
                    return;
                }

                const auto window_title = WSTR(dialog_title);
                const auto w_header = WSTR(header);
                const auto w_message = WSTR(message);

                DECLARE_STRUCT(TASKDIALOGCONFIG, dialog_config);

                dialog_config.cbSize = sizeof(TASKDIALOGCONFIG);
                dialog_config.dwCommonButtons = common_buttons;
                dialog_config.pszWindowTitle = window_title.c_str();
                dialog_config.pszMainIcon = TD_SHIELD_ICON;
                dialog_config.pszMainInstruction = w_header.c_str();
                dialog_config.pszContent = w_message.c_str();
                dialog_config.pszVerificationText = L"Do not show this message again.";
                dialog_config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;

                LOG_DEBUG("Launching task dialog: {}", header)

                BOOL flag_checked = FALSE;
                int chosen_button = -1;
                if (SUCCEEDED(TaskDialogIndirect(
                    &dialog_config,
                    &chosen_button,
                    nullptr,
                    &flag_checked
                ))) {
                    if (flag_checked) {
                        koalabox::cache::put(cache_key, false);
                    }

                    try {
                        LOG_DEBUG("Task dialog callback button: {}", chosen_button)
                        callback(chosen_button);
                    } catch (const Exception& e) {
                        LOG_ERROR("Task Dialog callback error: {}", e.what())
                    }
                }
            };

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

                winreg::RegKey exe_flags_key{
                    HKEY_CURRENT_USER,
                    L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers"
                };

                const auto run_as_admin = "RUNASADMIN";

                const auto flags_exp = exe_flags_key.TryGetStringValue(egl_path.wstring());
                const auto flags = flags_exp ? STR(flags_exp.GetValue()) : "";
                auto is_admin_configured = flags < contains > run_as_admin;

                const auto egl_path_32 =
                    egl_path.parent_path().parent_path() / "Win32" /
                    egl_path.filename();
                const auto flags_32_exp = exe_flags_key.TryGetStringValue(egl_path_32.wstring());
                const auto flags_32 = flags_32_exp ? STR(flags_32_exp.GetValue()) : "";

                const auto is_64 = egl_path.string() < contains > "Win64";
                if (is_64) {
                    // We need to set flags on 32-bit version as well so that it can
                    // launch 64-bit version.
                    is_admin_configured &= flags_32 < contains > run_as_admin;
                }

                if (is_admin_configured) {
                    return;
                }

                show_task_dialog(
                    "show_admin_setup_prompt",
                    "Set to always run as administrator?",
                    "Epic Games Launcher is detected to be running as administrator only "
                    "for the current session. Would you like to configure the Epic Game Store to "
                    "always run as administrator? (You can disable it later in the Compatibility tab "
                    "of the Properties window)",
                    TDCBF_YES_BUTTON | TDCBF_NO_BUTTON,
                    [&](int chosen_button) {
                        if (chosen_button == IDYES) {
                            const auto new_flags = WSTR(
                                fmt::format("{} {}", flags, run_as_admin)
                            );

                            LOG_DEBUG("Setting EGL flags: {}", STR(new_flags))

                            exe_flags_key.SetStringValue(
                                egl_path.wstring(),
                                new_flags
                            );

                            if (is_64) {
                                const auto new_flags_32 = WSTR(
                                    fmt::format("{} {}", flags_32, run_as_admin)
                                );

                                exe_flags_key.SetStringValue(
                                    egl_path_32.wstring(),
                                    new_flags_32
                                );
                            }
                        }
                    }
                );
            } else {
                show_task_dialog(
                    "show_proxy_config_prompt",
                    "Missing admin rights",
                    fmt::format(
                        "ScreamAPI could not automatically configure system proxy "
                        "because the Epic Games Launcher is not running as administrator. "
                        "Please restart the Epic Games Launcher as administrator to enable "
                        "automatic proxy configuration or configure the system proxy manually "
                        "with the following parameters:\n\n"
                        "Address: {}\nPort: {}\nBypass: {}",
                        localhost_ip, port, STR(bypass)
                    ),
                    TDCBF_CLOSE_BUTTON,
                    [](int _) {}
                );
            }
        };

        install_certificate();
        setup_system_proxy();
    }

    void init_store_mode(const Path& egl_path) {
        NEW_THREAD({
            try {
                LOG_INFO("🛍️ Detected store mode")

                const auto mitmproxy_path = setup_mitmproxy();
                run_mitmproxy(mitmproxy_path);
                configure_proxy(egl_path);
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

#include <store_mode/store_mode.hpp>
#include <scream_api/config.hpp>

#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>
#include <koalabox/paths.hpp>

#include <fstream>

namespace store_mode {
    void init_store_mode() {
        LOG_INFO("🛍️ Detected store mode")

        // Download https://snapshots.mitmproxy.org/9.0.1/mitmproxy-9.0.1-windows.zip

        // Verify installation:
        // mitmproxy --version

        // Call mitmproxy with script args

        // Group this and mitmproxy processes into a job to synchronize their lifetime
        // https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects
    }


}

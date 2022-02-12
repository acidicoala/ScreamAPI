#include "scream_api/scream_api.hpp"
#include "config/config.hpp"
#include "koalabox/logger/logger.hpp"

#include <sdk/eos_metrics.h>

using namespace koalabox;

DLL_EXPORT(EOS_EResult) EOS_Metrics_BeginPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_BeginPlayerSessionOptions* Options
) {
    static const auto EOS_Metrics_BeginPlayerSession_o = scream_api::get_original_function(
        &EOS_Metrics_BeginPlayerSession,
        __func__
    );

    if (config::instance.block_metrics) {
        logger::debug("⛔ Blocking EOS_Metrics_BeginPlayerSession");

        return EOS_EResult::EOS_Success;
    } else {
        return EOS_Metrics_BeginPlayerSession_o(Handle, Options);
    }
}

DLL_EXPORT(EOS_EResult) EOS_Metrics_EndPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_EndPlayerSessionOptions* Options
) {
    static const auto EOS_Metrics_EndPlayerSession_o = scream_api::get_original_function(
        &EOS_Metrics_EndPlayerSession,
        __func__
    );

    if (config::instance.block_metrics) {
        logger::debug("⛔ Blocking EOS_Metrics_EndPlayerSession");

        return EOS_EResult::EOS_Success;
    } else {
        return EOS_Metrics_EndPlayerSession_o(Handle, Options);
    }
}

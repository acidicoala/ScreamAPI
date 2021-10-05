#include <sdk/eos_metrics.h>
#include <scream_api/scream_api.hpp>
#include <config/config.hpp>
#include <logger/logger.hpp>

EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_BeginPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_BeginPlayerSessionOptions* Options
) {
    static auto proxy = scream_api::get_original_function(
        &EOS_Metrics_BeginPlayerSession, __func__
    );

    if (config::get().block_metrics) {
        logger::debug("⛔ Blocking EOS_Metrics_BeginPlayerSession");

        return EOS_EResult::EOS_Success;
    } else {
        return proxy(Handle, Options);
    }
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_EndPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_EndPlayerSessionOptions* Options
) {
    static auto proxy = scream_api::get_original_function(&EOS_Metrics_EndPlayerSession, __func__);

    if (config::get().block_metrics) {
        logger::debug("⛔ Blocking EOS_Metrics_EndPlayerSession");

        return EOS_EResult::EOS_Success;
    } else {
        return proxy(Handle, Options);
    }
}

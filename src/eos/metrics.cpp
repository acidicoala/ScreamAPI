#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/logger.hpp>

#include <sdk/eos_metrics.h>

using namespace scream_api;

DLL_EXPORT(EOS_EResult) EOS_Metrics_BeginPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_BeginPlayerSessionOptions* Options
) {
    GET_ORIGINAL_FUNCTION(EOS_Metrics_BeginPlayerSession)

    if (scream_api::config::instance.block_metrics) {
        LOG_DEBUG("⛔ Blocking EOS_Metrics_BeginPlayerSession")

        return EOS_EResult::EOS_Success;
    } else {
        return EOS_Metrics_BeginPlayerSession_o(Handle, Options);
    }
}

DLL_EXPORT(EOS_EResult) EOS_Metrics_EndPlayerSession(
    EOS_HMetrics Handle,
    const EOS_Metrics_EndPlayerSessionOptions* Options
) {
    GET_ORIGINAL_FUNCTION(EOS_Metrics_EndPlayerSession)

    if (scream_api::config::instance.block_metrics) {
        LOG_DEBUG("⛔ Blocking EOS_Metrics_EndPlayerSession")

        return EOS_EResult::EOS_Success;
    } else {
        return EOS_Metrics_EndPlayerSession_o(Handle, Options);
    }
}

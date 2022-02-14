cmake_minimum_required(VERSION 3.21)

include(KoalaBox/KoalaBox.cmake)

configure_globals(KoalaBox)

project(ScreamAPI VERSION 3.0.0)

set_32_and_64(ORIG_DLL EOSSDK-Win32-Shipping EOSSDK-Win64-Shipping)
configure_build_config(extra_build_config)

configure_version_resource("Free DLC for everyone ʕ ᵔᴥᵔʔ")

set_32_and_64(DLL_INPUT "${RES_DIR}/EOSSDK-Win32-Shipping.dll" "${RES_DIR}/EOSSDK-Win64-Shipping.dll")
set(
    EOS_SOURCES
    "${SRC_DIR}/eos/init.cpp"
    "${SRC_DIR}/eos/common.cpp"
    "${SRC_DIR}/eos/logging.cpp"
    "${SRC_DIR}/eos/metrics.cpp"
    "${SRC_DIR}/eos/ecom_misc.cpp"
    "${SRC_DIR}/eos/ecom_items.cpp"
    "${SRC_DIR}/eos/ecom_entitlements.cpp"
)
configure_exports_generator()
configure_linker_exports(
    "${ORIG_DLL}_o"
    "${DLL_INPUT}"
    "${SRC_DIR}/eos"
    ${EOS_SOURCES}
)

configure_library(
    SHARED

    ${SRC_DIR}/config/config.cpp
    ${EOS_SOURCES}
    ${SRC_DIR}/scream_api/scream_api.cpp
    ${SRC_DIR}/main.cpp

    ${KOALABOX_SRC_DIR}/koalabox/dll_monitor/dll_monitor.cpp
    ${KOALABOX_SRC_DIR}/koalabox/hook/hook.cpp
    ${KOALABOX_SRC_DIR}/koalabox/loader/loader.cpp
    ${KOALABOX_SRC_DIR}/koalabox/logger/logger.cpp
    ${KOALABOX_SRC_DIR}/koalabox/util/util.cpp
    ${KOALABOX_SRC_DIR}/koalabox/win_util/win_util.cpp

    ${LINKER_EXPORTS}
)

configure_precompile_headers(${SRC_DIR}/pch.hpp)

configure_output_name(${ORIG_DLL})

configure_include_directories(${CMAKE_SOURCE_DIR}/include)

configure_dependencies(spdlog nlohmann_json cpr PolyHook_2)

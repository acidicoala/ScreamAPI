cmake_minimum_required(VERSION 3.24)

project(ScreamAPI VERSION 4.0.0)

include(KoalaBox/cmake/KoalaBox.cmake)

set_32_and_64(EOSSDK_DLL EOSSDK-Win32-Shipping EOSSDK-Win64-Shipping)

configure_version_resource("Free DLC for everyone ʕ ᵔᴥᵔʔ")

set(
    EOS_SOURCES
    src/eos/init.cpp
    src/eos/logging.cpp
    src/eos/metrics.cpp
    src/eos/ecom_misc.cpp
    src/eos/ecom_items.cpp
    src/eos/ecom_entitlements.cpp
)

configure_linker_exports(
    UNDECORATE true
    FORWARDED_DLL "${EOSSDK_DLL}_o"
    INPUT_SOURCES_DIR "${CMAKE_SOURCE_DIR}/src/eos"
    INPUT_DLLS "${CMAKE_SOURCE_DIR}/res/${EOSSDK_DLL}.dll"
    DEP_SOURCES "${EOS_SOURCES}"
)

set(
    SCREAM_API_SOURCES
    ${EOS_SOURCES}
    src/game_mode/game_mode.cpp
    src/game_mode/game_mode.hpp
    src/scream_api/api.cpp
    src/scream_api/api.hpp
    src/scream_api/config.cpp
    src/scream_api/config.hpp
    src/scream_api/scream_api.cpp
    src/scream_api/scream_api.hpp
    src/store_mode/store_mode.cpp
    src/store_mode/store_mode.hpp
    src/legacy_linker_exports.h
    src/main.cpp
    ${GENERATED_LINKER_EXPORTS}
)

add_library(${PROJECT_NAME} SHARED ${SCREAM_API_SOURCES} ${VERSION_RESOURCE})

configure_include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(KoalaBox EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE KoalaBox)

# Embed addon script
file(READ src/store_mode/mitmproxy/egs_addon.py EGS_ADDON_UTF8 ENCODING UTF-8)
string(HEX "${EGS_ADDON_UTF8}" EGS_ADDON_HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," EGS_ADDON_HEX ${EGS_ADDON_HEX})

configure_build_config(extra_build_config)

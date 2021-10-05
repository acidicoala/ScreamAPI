cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

IF (DEFINED ENV{SCREAM_API_VERSION})
    set(SCREAM_API_VERSION "$ENV{SCREAM_API_VERSION}.0")
ELSE ()
    message(FATAL_ERROR "Environment variable SCREAM_API_VERSION must be set with format: x.x.x")
ENDIF ()

# Configure vcpkg triplet

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
if (${CMAKE_GENERATOR_PLATFORM} STREQUAL "Win32")
    set(VCPKG_TARGET_TRIPLET x86-windows-static CACHE STRING "VCPKG Target Triplet to use")
elseif (${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
    set(VCPKG_TARGET_TRIPLET x64-windows-static CACHE STRING "VCPKG Target Triplet to use")
else ()
    message(FATAL_ERROR "Unexpected argument for the platform generator")
endif ()

project(ScreamAPI VERSION ${SCREAM_API_VERSION})

# 0. Global config

set(PROJECT_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(VERSION_SUFFIX $ENV{VERSION_SUFFIX})

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)
set(GEN_DIR ${PROJECT_BINARY_DIR}/generated)
set(LINKER_EXPORTS ${GEN_DIR}/linker_exports.h)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set(EOSSDK_DLL EOSSDK-Win${BITS}-Shipping)
set(EOSSDK_DLL_PATH ${RES_DIR}/${EOSSDK_DLL}.dll)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 1. Build the exports generator executable

add_executable(exports_generator ${RES_DIR}/exports_generator.cpp)

# 2. Generate the linker exports

add_custom_command(
        OUTPUT ${LINKER_EXPORTS}
        COMMAND exports_generator # (Output from step 1)
        "${EOSSDK_DLL_PATH}" # Input DLL
        "${SRC_DIR}/eos" # Input sources
        "${LINKER_EXPORTS}" # Output header
        DEPENDS exports_generator "${EOSSDK_DLL_PATH}"
        "${SRC_DIR}/eos/init.cpp"
        "${SRC_DIR}/eos/logging.cpp"
        "${SRC_DIR}/eos/metrics.cpp"
        "${SRC_DIR}/eos/ecom_misc.cpp"
        "${SRC_DIR}/eos/ecom_items.cpp"
        "${SRC_DIR}/eos/ecom_entitlements.cpp"
)

# Make the linker_exports header available before build
file(MAKE_DIRECTORY ${GEN_DIR}) # Touch will fail if directory does not exist
file(TOUCH ${LINKER_EXPORTS})

# 3. Substitute variables in build and runtime config files

## Generate build config file
configure_file(${RES_DIR}/build_config.gen.h ${GEN_DIR}/build_config.h)

## Generate version resource file
configure_file(${RES_DIR}/version.gen.rc ${GEN_DIR}/version.rc)

# 4. Build the ScreamAPI library

add_library(
        ${CMAKE_PROJECT_NAME} SHARED
        ${SRC_DIR}/pch.hpp
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/win_util/win_util.cpp
        ${SRC_DIR}/util/util.cpp
        ${SRC_DIR}/loader/loader.cpp
        ${SRC_DIR}/config/config.cpp
        ${SRC_DIR}/logger/logger.cpp
        ${SRC_DIR}/scream_api/scream_api.cpp
        ${SRC_DIR}/eos/init.cpp
        ${SRC_DIR}/eos/common.cpp
        ${SRC_DIR}/eos/logging.cpp
        ${SRC_DIR}/eos/metrics.cpp
        ${SRC_DIR}/eos/ecom_misc.cpp
        ${SRC_DIR}/eos/ecom_items.cpp
        ${SRC_DIR}/eos/ecom_entitlements.cpp
        ${LINKER_EXPORTS}
        ${GEN_DIR}/version.rc
)

set_target_properties(
        ${CMAKE_PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${EOSSDK_DLL}
)

target_include_directories(
        ${CMAKE_PROJECT_NAME} PRIVATE
        ${SRC_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${GEN_DIR}
)

target_precompile_headers(
        ${CMAKE_PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${SRC_DIR}/pch.hpp>"
)

# Link dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

target_link_libraries(
        ${CMAKE_PROJECT_NAME} PRIVATE
        spdlog::spdlog
        cpr
        nlohmann_json
)
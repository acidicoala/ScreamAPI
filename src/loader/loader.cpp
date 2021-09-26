#include "loader.hpp"
#include "util/util.hpp"
#include "win_util/win_util.hpp"

using namespace loader;

FARPROC loader::get_original_function(std::string& function_name) {
    auto original_handle = get_original_module_handle();

    auto function_address = win_util::get_proc_address(original_handle, function_name.c_str());

    return function_address;
}

HMODULE loader::get_original_module_handle() {
    static auto original_module = win_util::get_module_handle(get_original_module_name());

    return original_module;
}

const char* loader::get_original_module_name() {
    static auto original_module_name =
        util::is_amd64()
            ? "EOSSDK-Win64-Shipping_o.dll"
            : "EOSSDK-Win32-Shipping_o.dll";

    return original_module_name;
}

HMODULE loader::get_this_module_handle() {
    static auto this_module = win_util::get_module_handle(get_this_module_name());

    return this_module;
}

const char* loader::get_this_module_name() {
    static auto this_module_name =
        util::is_amd64()
            ? "EOSSDK-Win64-Shipping.dll"
            : "EOSSDK-Win32-Shipping.dll";

    return this_module_name;
}

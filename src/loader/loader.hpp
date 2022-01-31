#pragma once

/**
 * Functions without arguments memoize the return value
 */
namespace loader {

    FARPROC get_original_function(std::string& function_name);

    HMODULE get_original_module_handle();

    const char* get_original_module_name();

    HMODULE get_this_module_handle();

    const char* get_this_module_name();

}
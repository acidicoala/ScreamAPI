#pragma once

namespace win_util {

    std::string format_message(DWORD message_id);

    std::string get_module_file_name(HMODULE handle);

    HMODULE get_module_handle(const char* module_name);

    HMODULE get_module_handle(std::string& module_name);

    FARPROC get_proc_address(HMODULE handle, LPCSTR procedure_name);

    bool free_library(HMODULE handle);

    HMODULE load_library(std::filesystem::path& module_path);

}
#include "win_util.hpp"
#include "logger/logger.hpp"
#include "util/util.hpp"

std::string win_util::format_message(DWORD message_id) {
    wchar_t buffer[1024];

    ::FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        message_id,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buffer,
        (sizeof(buffer) / sizeof(wchar_t)),
        nullptr
    );

    return util::to_string(buffer);
}

HMODULE win_util::get_module_handle(std::string& module_name) {
    return win_util::get_module_handle(module_name.c_str());
}

HMODULE win_util::get_module_handle(LPCSTR module_name) {
    auto handle = ::GetModuleHandleA(module_name);

    if (handle == nullptr) {
        util::panic(
            "win_util::get_module_handle",
            "Failed to get a handle of the '{}' module"_format(module_name)
        );
    }

    return handle;
}

std::string win_util::get_module_file_name(HMODULE handle) {
    constexpr auto buffer_size = 1024;
    auto buffer = new WCHAR[buffer_size];
    auto length = ::GetModuleFileNameW(handle, buffer, buffer_size);

    if (length == 0 or length == buffer_size) {
        util::panic(
            "win_util::get_module_file_name",
            "Failed to get the file name of the given module handle: {}"_format(fmt::ptr(handle))
        );
    }

    return util::to_string(std::wstring(buffer));
}

FARPROC win_util::get_proc_address(HMODULE handle, LPCSTR procedure_name) {
    auto address = ::GetProcAddress(handle, procedure_name);

    if (address == nullptr) {
        util::panic(
            "win_util::get_proc_address",
            "Failed to get the address of the '{}' procedure"_format(procedure_name)
        );
    }

    return address;
}

bool win_util::free_library(HMODULE handle) {
    auto successful = ::FreeLibrary(handle);

    if (not successful) {
        logger::error(
            "Failed to free a library with the given module handle: {}", fmt::ptr(handle)
        );
    }

    return successful;
}

HMODULE win_util::load_library(std::filesystem::path& module_path) {
    auto handle = ::LoadLibraryW(module_path.wstring().c_str());

    if (handle == nullptr) {
        util::panic(
            "win_util::load_library",
            "Failed to load the module at path: '{}'"_format(module_path.string())
        );
    }

    return handle;
}

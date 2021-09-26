#pragma once

/**
 * Functions without arguments memoize the return value
 */
namespace util {

    void error_box(std::string title, std::string message);

    std::filesystem::path get_working_dir();

    bool is_amd64();

    void panic(std::string title, std::string message);

    std::string to_string(const std::wstring& wstr);

    std::wstring to_wstring(const std::string& str);


}

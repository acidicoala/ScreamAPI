// Windows headers
#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <Windows.h>

// C++ std lib headers
#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <regex>

// Ensure that assert macros work
#undef NDEBUG

using namespace std;

bool is_amd64();

vector<string> get_implemented_functions(filesystem::path path);

vector<string> get_exported_functions(filesystem::path& dll_path);

vector<string> get_legacy_functions();

/**
 * Args <br>
 * 0: program name <br>
 * 1: dll input path <br>
 * 1: eos sources path <br>
 * 2: header output path <br>
 */
int wmain(const int argc, const wchar_t* argv[]) {
    assert(argc == 4);

    filesystem::path dll_input_path(argv[1]);
    filesystem::path eos_sources_path(argv[2]);
    filesystem::path header_output_path(argv[3]);

    auto implemented_functions = get_implemented_functions(eos_sources_path);

    auto exported_functions = get_exported_functions(dll_input_path);

    auto legacy_functions = get_legacy_functions();
    exported_functions.insert(
        exported_functions.end(),
        legacy_functions.begin(),
        legacy_functions.end()
    );

    auto forwarded_dll_name =
        is_amd64()
            ? "EOSSDK-Win64-Shipping_o"
            : "EOSSDK-Win32-Shipping_o";

    // Create directories for export header, if necessary
    filesystem::create_directories(header_output_path.parent_path());

    // Open the export header file for writing
    ofstream export_file(header_output_path, ofstream::out | ofstream::trunc);
    assert(export_file.is_open());

    // Add header guard
    export_file << "#pragma once" << endl;

    // Iterate over exported functions
    for (const auto& name: exported_functions) {
        // Iterate over implemented functions and check if it is there
        for (const auto& implemented_function: implemented_functions) {
            bool is_implemented = is_amd64()
                ? name == implemented_function
                : name.find("_" + implemented_function + "@") != string::npos;

            if (is_implemented) {
                export_file << "// ";
            }
        }

        export_file << "#pragma comment(linker, " << '"' << "/export:" << name << '='
                    << forwarded_dll_name << '.' << name << '"' << ')' << endl;
    }
}

// Adapted from: https://github.com/mborne/dll2def/blob/master/dll2def.cpp
vector<string> get_exported_functions(filesystem::path& dll_path) {
    auto exported_functions = vector<string>();

    HMODULE lib = LoadLibraryExW(
        dll_path.c_str(),
        nullptr,
        DONT_RESOLVE_DLL_REFERENCES
    );

    assert(((PIMAGE_DOS_HEADER) lib)->e_magic == IMAGE_DOS_SIGNATURE);
    auto header = (PIMAGE_NT_HEADERS) ((BYTE*) lib + ((PIMAGE_DOS_HEADER) lib)->e_lfanew);
    assert(header->Signature == IMAGE_NT_SIGNATURE);
    assert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
    auto exports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
        (BYTE*) lib +
        header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
    );
    PVOID names = (BYTE*) lib + exports->AddressOfNames;

    // Iterate over the names and add them to the vector
    for (unsigned int i = 0; i < exports->NumberOfNames; i++) {
        auto name = (char*) lib + ((DWORD*) names)[i];
        exported_functions.emplace_back(name);
    }

    return exported_functions;
}

vector<string> get_legacy_functions() {
    return is_amd64()
        ? vector<string>{
            "EOS_AccountId_FromString",
            "EOS_AccountId_IsValid",
            "EOS_AccountId_ToString",
        }
        : vector<string>{
            "_EOS_AccountId_FromString@4",
            "_EOS_AccountId_IsValid@4",
            "_EOS_AccountId_ToString@12",
            "_EOS_Auth_AddNotifyLoginStatusChanged@12", // No of args changed from 3 to 4
            "_EOS_Auth_CopyUserAuthToken@12", // No of args changed from 3 to 4
        };
}

/**
 * Returns a list of functions parsed from the sources
 * in a given directory. Edge cases: Comments
 */
vector<string> get_implemented_functions(filesystem::path path) {
    vector<string> implemented_functions;

    for (auto& p: filesystem::recursive_directory_iterator(path)) {
        auto file_path = p.path();

        ifstream ifs(file_path);
        std::string file_content(istreambuf_iterator<char>{ifs}, {});

        // Matches function name in the 1st group
        static regex func_name_pattern(R"(\s*EOS_DECLARE_FUNC\(\w+\)\s*(\w+)\s*\()");
        smatch match;
        while (regex_search(file_content, match, func_name_pattern)) {
            auto func_name = match.str(1);
            implemented_functions.push_back(func_name);
            cout << "Implemented: " << func_name << endl;
            file_content = match.suffix();
        }
    }

    return implemented_functions;
}

bool is_amd64() {
    // Static variables are used to suppress
    // "Condition always true/false" warnings
#ifdef _WIN64
    static auto result = true;
#else
    static auto result = false;
#endif
    return result;
}

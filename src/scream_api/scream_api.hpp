#pragma once

#include <sdk/eos_base.h>
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"

#define DLL_EXPORT(TYPE) extern "C" _declspec(dllexport) TYPE

namespace scream_api {
    using namespace koalabox;

    extern std::string namespace_id;

    extern HMODULE original_module;

    void init(HMODULE self_module);

    void shutdown();

    template<typename RetType, typename... ArgTypes>
    auto get_original_function(
        RetType(EOS_CALL*)(ArgTypes...),
        LPCSTR undecorated_function_name,
        int arg_bytes = 0
    ) {
        // No idea what how this works 🙃
        using func_type = RetType(EOS_CALL*)(ArgTypes...);

        if (arg_bytes == 0) {
            // Guess byte count
            arg_bytes = 4 * sizeof...(ArgTypes);
        }

        std::string function_name =
            util::is_64_bit()
                ? undecorated_function_name
                : fmt::format("_{}@{}", undecorated_function_name, arg_bytes);

        // Get C-style pointer to function
        auto function_address = win_util::get_proc_address(
            scream_api::original_module,
            function_name.c_str()
        );

        // Cast it to target function type
        return reinterpret_cast<func_type>(function_address);
    }
}

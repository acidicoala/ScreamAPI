#pragma once

#include <sdk/eos_base.h>
#include "loader/loader.hpp"
#include "util/util.hpp"

namespace scream_api {
    extern std::string namespace_id;

    void init();

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

        std::string functionName =
            util::is_amd64()
                ? undecorated_function_name
                : fmt::format("_{}@{}", undecorated_function_name, arg_bytes);

        // Get C-style pointer to function
        auto function_address = loader::get_original_function(functionName);

        // Cast it to target function type
        return reinterpret_cast<func_type>(function_address);
    }
}

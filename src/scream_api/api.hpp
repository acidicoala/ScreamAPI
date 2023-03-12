#pragma once

#include <koalabox/http_client.hpp>

namespace api {

    std::optional<Map<String, String>> fetch_entitlements(const String& namespace_id);

}
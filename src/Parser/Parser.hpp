/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Parser
*/

#pragma once

#include "Macro.hpp"
#include "Error.hpp"
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <cstddef>
#include <unordered_map>

namespace raytracer::parser {

template <typename JsonValue>
using JsonBase = std::variant<
    std::nullptr_t,
    int,
    bool,
    double,
    std::string,
    std::vector<JsonValue>,
    std::unordered_map<std::string, JsonValue>
>;

struct JsonProto{
    using type = JsonBase<JsonProto>;
};

using JsonValue = JsonProto::type;

JsonValue parseJson(const char * RESTRICT filepath);

}// namespace raytracer::parser

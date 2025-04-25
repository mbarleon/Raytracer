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
#include <sstream>
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

struct JsonProto {
    using type = JsonBase<JsonProto>;
    type value;

    constexpr explicit JsonProto() = default;

    template <typename T>
    JsonProto(T &&val) : value(std::forward<T>(val)) {}

    explicit operator JsonProto::type() const
    {
        return value;
    }
};

using JsonValue = JsonProto::type;

using Iterator = std::string::const_iterator;

JsonValue parseJson(const char * RESTRICT filepath);

}// namespace raytracer::parser

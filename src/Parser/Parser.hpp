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

unit_static void skipWhitespace(Iterator &it, const Iterator &end);
unit_static char get(Iterator &it, const Iterator &end);
unit_static char peek(Iterator it, const Iterator &end);
unit_static void expect(Iterator &it, const Iterator &end, char expected);
unit_static JsonValue parseNull(Iterator &it, const Iterator &end);
unit_static JsonValue parseBool(Iterator &it, const Iterator &end);
unit_static JsonValue parseNumber(Iterator &it, const Iterator &end);
unit_static JsonValue parseString(Iterator &it, const Iterator &end);
unit_static JsonValue parseArray(Iterator &it, const Iterator &end);
unit_static JsonValue parseObject(Iterator &it, const Iterator &end);
unit_static JsonValue parseValue(Iterator &it, const Iterator &end);

}// namespace raytracer::parser

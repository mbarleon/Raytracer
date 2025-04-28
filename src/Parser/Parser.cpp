/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Parser
*/

#include "Parser.hpp"
#include "Error.hpp"
#include <fstream>
#include <sstream>

#if defined(UNIT_TESTS)
    namespace raytracer::parser {
#endif

unit_static char get(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end)
{
    if (it == end) {
        throw raytracer::exception::Error("raytracer::parser::get", "Unexpected end of input");
    }
    return *it++;
}

unit_static char peek(const raytracer::parser::Iterator it, const raytracer::parser::Iterator &end)
{
    if (it == end) {
        throw raytracer::exception::Error("raytracer::parser::peek", "Unexpected end of input");
    }
    return *it;
}

unit_static void  expect(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end, char expected)
{
    if (const char c = get(it, end); c != expected) {
        throw raytracer::exception::Error("raytracer::parser::expect", "Expected '", expected, "', got '", c, "'");
    }
}

unit_static void skipWhitespace(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    while (it != end) {
        if (std::distance(it, end) >= 2 && std::string(it, it + 2) == "//") {
            it += 2;
            while (it != end && *it != '\n') {
                ++it;
            }
        } else if (std::distance(it, end) >= 2 && std::string(it, it + 2) == "/*") {
            it += 2;
            while (std::distance(it, end) >= 2 && std::string(it, it + 2) != "*/") {
                ++it;
            }
            if (std::distance(it, end) >= 2 && std::string(it, it + 2) == "*/") {
                ++it;
            }
        } else if (!std::isspace(*it)) {
            break;
        }
        ++it;
    }
}

unit_static raytracer::parser::JsonValue  parseNull(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end)
{
    if (std::distance(it, end) >= 4 && std::string(it, it + 4) == "null") {
        std::advance(it, 4);
        return nullptr;
    }
    throw raytracer::exception::Error("raytracer::parser::parseNull", "Expected 'null'");
}

unit_static raytracer::parser::JsonValue  parseBool(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end)
{
    if (std::distance(it, end) >= 4 && std::string(it, it + 4) == "true") {
        std::advance(it, 4);
        return true;
    }
    if (std::distance(it, end) >= 5 && std::string(it, it + 5) == "false") {
        std::advance(it, 5);
        return false;
    }
    throw raytracer::exception::Error("raytracer::parser::parseBool", "Expected 'true' or 'false'");
}

unit_static raytracer::parser::JsonValue  parseNumber(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end)
{
    const raytracer::parser::Iterator start = it;
    bool isFloat = false;

    if (it != end && *it == '-') {
        ++it;
    }
    while (it != end && std::isdigit(*it)) {
        ++it;
    }
    if (it != end && *it == '.') {
        isFloat = true;
        ++it;
        while (it != end && std::isdigit(*it)) {
            ++it;
        }
    }
    const std::string numStr(start, it);
    try {
        if (isFloat) {
            return std::stod(numStr);
        }
        return std::stoi(numStr);
    } catch (const std::invalid_argument &) {
        throw raytracer::exception::Error("raytracer::parser::parseNumber", "Invalid number");
    } catch (const std::out_of_range &) {
        throw raytracer::exception::Error("raytracer::parser::parseNumber", "Invalid number");
    }
}

unit_static raytracer::parser::JsonValue parseString(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end)
{
    std::string result;
    bool terminated = false;

    expect(it, end, '"');
    while (it != end) {
        const char c = get(it, end);
        if (c == '"') {
            terminated = true;
            break;
        }
        if (c == '\\') {
            switch (get(it, end)) {
                case '\\': result += '\\'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: throw raytracer::exception::Error("raytracer::parser::parseString", "Invalid escape sequence");
            }
        } else {
            result += c;
        }
    }
    if (!terminated) {
        throw raytracer::exception::Error("raytracer::parser::parseString", "Unterminated string");
    }
    return result;
}

unit_static raytracer::parser::JsonValue parseValue(raytracer::parser::Iterator &it,
    const raytracer::parser::Iterator &end);

unit_static raytracer::parser::JsonValue parseArray(raytracer::parser::Iterator &it, // NOLINT(*-no-recursion)
    const raytracer::parser::Iterator &end)
{
    std::vector<raytracer::parser::JsonProto> array;

    expect(it, end, '[');
    skipWhitespace(it, end);
    if (peek(it, end) == ']') {
        ++it;
        return nullptr;
    }
    while (true) {
        skipWhitespace(it, end);
        array.emplace_back(parseValue(it, end));
        skipWhitespace(it, end);
        if (peek(it, end) == ']') {
            ++it;
            break;
        }
    }
    return array;
}

unit_static raytracer::parser::JsonValue parseObject(raytracer::parser::Iterator &it, // NOLINT(*-no-recursion)
    const raytracer::parser::Iterator &end)
{
    std::unordered_map<std::string, raytracer::parser::JsonProto> object;

    expect(it, end, '{');
    skipWhitespace(it, end);
    if (peek(it, end) == '}') {
        ++it;
        return nullptr;
    }
    while (true) {
        skipWhitespace(it, end);
        raytracer::parser::JsonValue key = parseString(it, end);
        if (std::get<std::string>(key).empty()) {
            throw raytracer::exception::Error("raytracer::parser::parseObject", "Expected string key");
        }
        skipWhitespace(it, end);
        expect(it, end, ':');
        skipWhitespace(it, end);
        object[std::get<std::string>(key)] = raytracer::parser::JsonProto(parseValue(it, end));
        skipWhitespace(it, end);
        if (peek(it, end) == '}') {
            ++it;
            break;
        }
    }
    return object;
}

unit_static raytracer::parser::JsonValue parseValue(raytracer::parser::Iterator &it, // NOLINT(*-no-recursion)
    const raytracer::parser::Iterator &end)
{
    skipWhitespace(it, end);
    raytracer::parser::JsonValue object;

    if (char c = peek(it, end); std::isdigit(c)) {
        object = parseNumber(it, end);
    } else {
        switch (c) {
            case '"':
                object = parseString(it, end);
                break;
            case '-':
                object = parseNumber(it, end);
                break;
            case 'n':
                object = parseNull(it, end);
                break;
            case 't': case 'f':
                object = parseBool(it, end);
                break;
            case '[':
                object = parseArray(it, end);
                break;
            case '{':
                object = parseObject(it, end);
                break;
            default:
                throw raytracer::exception::Error("parseValue", "Unknown value", c);
        }
    }
    expect(it, end, ',');
    return object;
}

#if defined(UNIT_TESTS)
    };
#endif

raytracer::parser::JsonValue raytracer::parser::parseJson(const char *RESTRICT filepath)
{
    std::stringstream ss;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw exception::Error("raytracer::parser::parseJson", "Could not open ", filepath);
    }
    ss << file.rdbuf();
    const std::string content = ss.str();
    Iterator it = content.begin();
    const Iterator end = content.end();
    const JsonValue result = parseValue(it, end);
    skipWhitespace(it, end);
    if (it != end) {
        throw exception::Error("raytracer::parser::parseJson", "Unexpected trailing data in ", filepath);
    }
    return result;
}

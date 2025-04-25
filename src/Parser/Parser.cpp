/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Parser
*/

#include "Parser.hpp"

static void skipWhitespace(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    while (it != end && std::isspace(*it))
        ++it;
}

static char get(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    if (it == end)
        throw raytracer::exception::Error("raytracer::parser::get", "Unexpected end of input");
    return *it++;
}

static char peek(raytracer::parser::Iterator it, const raytracer::parser::Iterator &end)
{
    if (it == end)
        throw raytracer::exception::Error("raytracer::parser::peek", "Unexpected end of input");
    return *it;
}

static void expect(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end, char expected)
{
    char c = get(it, end);

    if (c != expected)
        throw raytracer::exception::Error("raytracer::parser::expect", "Expected '", expected, "', got '", c, "'");
}

static raytracer::parser::JsonValue parseNull(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    if (std::distance(it, end) >= 4 && std::string(it, it + 4) == "null") {
        std::advance(it, 4);
        return nullptr;
    }
    throw raytracer::exception::Error("raytracer::parser::parseNull", "Expected 'null'");
}

static raytracer::parser::JsonValue parseBool(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    if (std::distance(it, end) >= 4 && std::string(it, it + 4) == "true") {
        std::advance(it, 4);
        return true;
    } else if (std::distance(it, end) >= 5 && std::string(it, it + 5) == "false") {
        std::advance(it, 5);
        return false;
    }
    throw raytracer::exception::Error("raytracer::parser::parseBool", "Expected 'true' or 'false'");
}

static raytracer::parser::JsonValue parseNumber(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    auto start = it;
    bool isFloat = false;

    if (*it == '-')
        ++it;
    while (it != end && std::isdigit(*it))
        ++it;
    if (it != end && *it == '.') {
        isFloat = true;
        ++it;
        while (it != end && std::isdigit(*it)) ++it;
    }
    std::string numStr(start, it);
    try {
        if (isFloat)
            return std::stod(numStr);
        return std::stoi(numStr);
    } catch (const std::invalid_argument &e) {
        throw raytracer::exception::Error("raytracer::parser::parseNumber", "Invalid number: ", numStr);
    } catch (const std::out_of_range &e) {
        throw raytracer::exception::Error("raytracer::parser::parseNumber", "Invalid number: ", numStr);
    }
}

static raytracer::parser::JsonValue parseString(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
{
    std::string result;

    expect(it, end, '"');
    while (it != end) {
        char c = get(it, end);
        if (c == '"')
            break;
        if (c == '\\') {
            char esc = get(it, end);
            switch (esc) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
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
    return result;
}

static raytracer::parser::JsonValue parseValue(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end);

static raytracer::parser::JsonValue parseArray(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
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
        array.push_back(parseValue(it, end));
        skipWhitespace(it, end);
        if (peek(it, end) == ']') {
            ++it;
            break;
        }
        expect(it, end, ',');
    }
    return array;
}

static raytracer::parser::JsonValue parseObject(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end)
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
        if (!std::holds_alternative<std::string>(key))
            raytracer::exception::Error("raytracer::parser::parseObject", "Expected string key");
        skipWhitespace(it, end);
        expect(it, end, ':');
        skipWhitespace(it, end);
        object[std::get<std::string>(key)] = parseValue(it, end);
        skipWhitespace(it, end);
        if (peek(it, end) == '}') {
            ++it;
            break;
        }
        expect(it, end, ',');
    }
    return object;
}

static raytracer::parser::JsonValue parseValue(raytracer::parser::Iterator &it, const raytracer::parser::Iterator &end) {
    skipWhitespace(it, end);
    char c = peek(it, end);
    if (std::isdigit(c))
        return parseNumber(it, end);
    switch (c) {
        case '"':
            return parseString(it, end);
        case '-':
            return parseNumber(it, end);
        case 'n':
            return parseNull(it, end);
        case 't': case 'f':
            return parseBool(it, end);
        case '[':
            return parseArray(it, end);
        case '{':
            return parseObject(it, end);
        default:
            throw raytracer::exception::Error("parseValue", "Unknown value", c);
    }
}

raytracer::parser::JsonValue raytracer::parser::parseJson(const char *RESTRICT filepath)
{
    std::stringstream ss;
    std::ifstream file(filepath);

    if (!file.is_open())
        throw exception::Error("raytracer::parser::parseJson", "Could not open ", filepath);
    ss << file.rdbuf();
    std::string content = ss.str();
    Iterator it = content.begin();
    Iterator end = content.end();
    JsonValue result = parseValue(it, end);
    skipWhitespace(it, end);
    if (it != end)
        throw exception::Error("raytracer::parser::parseJson", "Unexpected trailing data in ", filepath);
    return result;
}

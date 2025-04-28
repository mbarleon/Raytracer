/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Parser
*/

#include "Error.hpp"
#include "../src/Parser/Parser.hpp"
#include "criterion/criterion.h"
#include <fstream>

Test(SkipWhitespace, NoWhitespace)
{
    const std::string test = "abcd";
    auto begin = test.begin();
    const auto end = test.end();

    raytracer::parser::skipWhitespace(begin, end);
    cr_assert_eq(*begin, 'a');
}

Test(SkipWhitespace, BeginWhitespace)
{
    const std::string test = "     abcd";
    auto begin = test.begin();
    const auto end = test.end();

    raytracer::parser::skipWhitespace(begin, end);
    cr_assert_eq(*begin, 'a');
}

Test(SkipWhitespace, MiddleWhitespace)
{
    const std::string test = "ab    cd";
    auto begin = test.begin();
    const auto end = test.end();

    raytracer::parser::skipWhitespace(begin, end);
    cr_assert_eq(*begin, 'a');
}

Test(SkipWhitespace, EndWhitespace)
{
    const std::string test = "abcd    ";
    auto begin = test.begin();
    const auto end = test.end();

    raytracer::parser::skipWhitespace(begin, end);
    cr_assert_eq(*begin, 'a');
}

Test(SkipWhitespace, OnlyWhitespace)
{
    const std::string test = "                 ";
    auto begin = test.begin();
    const auto end = test.end();

    raytracer::parser::skipWhitespace(begin, end);
    cr_assert_eq(begin, end);
}

Test(get, normal)
{
    const std::string test = "a";
    auto begin = test.begin();
    const auto end = test.end();
    const char ret = raytracer::parser::get(begin, end);

    cr_assert_eq(ret, 'a');
    cr_assert_eq(begin, end);
}

Test(get, end)
{
    const std::string test;
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::get(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::get");
        cr_assert_str_eq(e.what(), "Unexpected end of input");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(peek, normal)
{
    const std::string test = "a";
    const auto begin = test.begin();
    const auto end = test.end();
    const char ret = raytracer::parser::peek(begin, end);

    cr_assert_eq(ret, 'a');
    cr_assert_eq(begin, test.begin());
}

Test(peek, end)
{
    const std::string test;
    const auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::peek(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::peek");
        cr_assert_str_eq(e.what(), "Unexpected end of input");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(expect, normal)
{
    const std::string test = "a";
    auto begin = test.begin();
    const auto end = test.end();
    try {
        constexpr char expected = 'a';
        raytracer::parser::expect(begin, end, expected);
        cr_assert(true);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(expect, wrongChar)
{
    const std::string test = "a";
    auto begin = test.begin();
    const auto end = test.end();
    try {
        constexpr char expected = 'b';
        raytracer::parser::expect(begin, end, expected);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::expect");
        cr_assert_str_eq(e.what(), "Expected 'b', got 'a'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(expect, end)
{
    const std::string test;
    auto begin = test.begin();
    const auto end = test.end();

    try {
        constexpr char expected = 'a';
        raytracer::parser::expect(begin, end, expected);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::get");
        cr_assert_str_eq(e.what(), "Unexpected end of input");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNull, null)
{
    const std::string test = "null";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseNull(begin, end);
        cr_assert(std::holds_alternative<std::nullptr_t>(ret));
        cr_assert_eq(begin, test.begin() + 4);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNull, null_too_small)
{
    const std::string test = "nul";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseNull(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseNull");
        cr_assert_str_eq(e.what(), "Expected 'null'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseBool, True)
{
    const std::string test = "true";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseBool(begin, end);
        cr_assert(std::holds_alternative<bool>(ret));
        cr_assert_eq(std::get<bool>(ret), true);
        cr_assert_eq(begin, test.begin() + 4);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseBool, Fasle)
{
    const std::string test = "false";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseBool(begin, end);
        cr_assert(std::holds_alternative<bool>(ret));
        cr_assert_eq(std::get<bool>(ret), false);
        cr_assert_eq(begin, test.begin() + 5);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseBool, Wrong_false)
{
    const std::string test = "fffff";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseBool(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseBool");
        cr_assert_str_eq(e.what(), "Expected 'true' or 'false'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseBool, Wrong_true)
{
    const std::string test = "tttt";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseBool(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseBool");
        cr_assert_str_eq(e.what(), "Expected 'true' or 'false'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseBool, too_small)
{
    const std::string test = "aa";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const raytracer::parser::JsonValue ret = raytracer::parser::parseBool(begin, end);
        cr_assert_fail("Expected raytracer::exception::Error, but no exception was thrown");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseBool");
        cr_assert_str_eq(e.what(), "Expected 'true' or 'false'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNumber, positive_integer)
{
    const std::string test = "42";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseNumber(begin, end);
        cr_assert(std::holds_alternative<int>(ret));
        cr_assert_eq(std::get<int>(ret), 42);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNumber, negative_float)
{
    const std::string test = "-3.14";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseNumber(begin, end);
        cr_assert(std::holds_alternative<double>(ret));
        cr_assert_float_eq(std::get<double>(ret), -3.14, 0.0001);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNumber, invalid_number)
{
    const std::string test = "abc";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseNumber(begin, end);
        cr_assert_fail("Expected exception for invalid number");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseNumber");
        cr_assert_str_eq(e.what(), "Invalid number");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseNumber, number_out_of_range)
{
    const std::string test = "99999999999999999999999999999999";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseNumber(begin, end);
        cr_assert_fail("Expected exception for invalid number");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseNumber");
        cr_assert_str_eq(e.what(), "Invalid number");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseString, simple_string)
{
    const std::string test = "\"hello\"";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseString(begin, end);
        cr_assert(std::holds_alternative<std::string>(ret));
        cr_assert_str_eq(std::get<std::string>(ret).c_str(), "hello");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseString, string_with_escape)
{
    const std::string test = R"("line\n\\\b\f\r\tbreak")";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseString(begin, end);
        cr_assert(std::holds_alternative<std::string>(ret));
        cr_assert_str_eq(std::get<std::string>(ret).c_str(), "line\n\\\b\f\r\tbreak");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseString, invalid_escape)
{
    const std::string test = R"("unterminat\ked")";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseString(begin, end);
        cr_assert_fail("Expected exception for unterminated string");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseString");
        cr_assert_str_eq(e.what(), "Invalid escape sequence");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseString, unterminated_string)
{
    const std::string test = "\"unterminated";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseString(begin, end);
        cr_assert_fail("Expected exception for unterminated string");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseString");
        cr_assert_str_eq(e.what(), "Unterminated string");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseArray, empty_array)
{
    const std::string test = "[]";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const auto ret = raytracer::parser::parseArray(begin, end);
        cr_assert(std::holds_alternative<std::nullptr_t>(ret));
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseArray, array_of_numbers)
{
    const std::string test = "[1, 2, 3]";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseArray(begin, end);
        cr_assert(std::holds_alternative<std::vector<raytracer::parser::JsonProto>>(ret));
        auto vec = std::get<std::vector<raytracer::parser::JsonProto>>(ret);
        cr_assert_eq(vec.size(), 3);
        cr_assert(std::holds_alternative<int>(vec[0].value));
        cr_assert_eq(std::get<int>(vec[0].value), 1);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseObject, empty_object)
{
    const std::string test = "{}";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const auto ret = raytracer::parser::parseObject(begin, end);
        cr_assert(std::holds_alternative<std::nullptr_t>(ret));
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseObject, simple_key_value)
{
    const std::string test = "{\"key\": 123}";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseObject(begin, end);
        const bool alt = std::holds_alternative<std::unordered_map<std::string, raytracer::parser::JsonProto>>(ret);
        cr_assert(alt);
        auto obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(ret);
        cr_assert(obj.contains("key"));
        cr_assert(std::holds_alternative<int>(obj["key"].value));
        cr_assert_eq(std::get<int>(obj["key"].value), 123);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseObject, non_string_key_should_throw)
{
    const std::string test = "{123: \"value\"}";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseObject(begin, end);
        cr_assert_fail("Expected exception due to non-string key");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::expect");
        cr_assert_str_eq(e.what(), "Expected '\"', got '1'");
    } catch (...) {
        cr_assert_fail("Unexpected exception type");
    }
}

Test(parseObject, empty_string_key_should_throw)
{
    const std::string test = R"({"": "value"})";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseObject(begin, end);
        cr_assert_fail("Expected exception due to non-string key");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseObject");
        cr_assert_str_eq(e.what(), "Expected string key");
    }catch (...) {
        cr_assert_fail("Unexpected exception type");
    }
}

Test(parseValue, value_string)
{
    const std::string test = "\"hello\"";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<std::string>(ret));
        cr_assert_str_eq(std::get<std::string>(ret).c_str(), "hello");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_number)
{
    const std::string test = "100";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<int>(ret));
        cr_assert_eq(std::get<int>(ret), 100);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_negative_number)
{
    const std::string test = "-100";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<int>(ret));
        cr_assert_eq(std::get<int>(ret), -100);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_null)
{
    const std::string test = "null";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        const auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<std::nullptr_t>(ret));
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_true)
{
    const std::string test = "true";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<bool>(ret));
        cr_assert(std::get<bool>(ret) == true);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_false)
{
    const std::string test = "false";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<bool>(ret));
        cr_assert(std::get<bool>(ret) == false);
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_array)
{
    const std::string test = "[\"x\", 2]";
    auto begin = test.begin();
    auto end = test.end();

    try {
        auto ret = raytracer::parser::parseValue(begin, end);
        cr_assert(std::holds_alternative<std::vector<raytracer::parser::JsonProto>>(ret));
        auto vec = std::get<std::vector<raytracer::parser::JsonProto>>(ret);
        cr_assert_eq(vec.size(), 2);
        cr_assert(std::holds_alternative<std::string>(vec[0].value));
        cr_assert_str_eq(std::get<std::string>(vec[0].value).c_str(), "x");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseValue, value_invalid)
{
    const std::string test = "@oops";
    auto begin = test.begin();
    const auto end = test.end();

    try {
        raytracer::parser::parseValue(begin, end);
        cr_assert_fail("Expected exception for unknown value");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "parseValue");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseJson, valid_json_file)
{
    const auto filename = "test_valid.json";
    std::ofstream file(filename);
    file << R"({"name": "Mathieu", "value": 10})";
    file.close();

    try {
        auto ret = raytracer::parser::parseJson(filename);
        const bool alt = std::holds_alternative<std::unordered_map<std::string, raytracer::parser::JsonProto>>(ret);
        cr_assert(alt);
        auto obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(ret);
        cr_assert(obj.contains("name"));
        cr_assert(obj.contains("value"));
    } catch (...) {
        cr_assert_fail("Unexpected exception while parsing valid JSON");
    }

    std::remove(filename);
}

Test(parseJson, file_not_found)
{
    try {
        const auto filename = "nonexistent_file.json";
        raytracer::parser::parseJson(filename);
        cr_assert_fail("Expected exception for file not found");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseJson");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }
}

Test(parseJson, file_with_trailing_data)
{
    const auto filename = "test_trailing.json";
    std::ofstream file(filename);
    file << "123 true";
    file.close();

    try {
        raytracer::parser::parseJson(filename);
        cr_assert_fail("Expected exception due to trailing data");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "raytracer::parser::parseJson");
    } catch (...) {
        cr_assert_fail("Unexpected exception type thrown");
    }

    std::remove(filename);
}

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

#define ASSERT_THROW_ERROR(statement, expected_where, expected_what)   \
    try {                                                              \
        statement;                                                     \
        cr_assert_fail("Expected exception but none thrown");          \
    } catch (const raytracer::exception::Error &e) {                   \
        cr_assert_str_eq(e.where(), expected_where);                   \
        cr_assert_str_eq(e.what(), expected_what);                     \
    }

TestSuite(SkipWhitespace);

Test(SkipWhitespace, no_whitespace)
{
    const std::string s = "abcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}

Test(SkipWhitespace, leading_whitespace)
{
    const std::string s = "    abcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}

Test(SkipWhitespace, only_whitespace)
{
    const std::string s = "    ";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(it, s.end());
}

Test(SkipWhitespace, single_line_comment)
{
    const std::string s = "// this is a comment\nabcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}

Test(SkipWhitespace, single_line_comment_eof)
{
    const std::string s = "// comment without newline";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, '\0');
}

Test(SkipWhitespace, multi_line_comment)
{
    const std::string s = "/* this is a\nmulti-line comment */abcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}

Test(SkipWhitespace, multi_line_comment_eof)
{
    const std::string s = "/* unterminated comment";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(it, s.end());
}

Test(SkipWhitespace, leading_spaces_and_comments)
{
    const std::string s = "   // comment\n   /* another comment */    abcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}

Test(SkipWhitespace, mixed_comments_and_text)
{
    const std::string s = "/*comment*/ // another\n  /*again*/abcd";
    auto it = s.begin();
    raytracer::parser::skipWhitespace(it, s.end());
    cr_assert_eq(*it, 'a');
}


TestSuite(GetPeekExpect);

Test(GetPeekExpect, get_valid)
{
    const std::string s = "x";
    auto it = s.begin();
    cr_assert_eq(raytracer::parser::get(it, s.end()), 'x');
    cr_assert_eq(it, s.end());
}

Test(GetPeekExpect, get_empty)
{
    const std::string s;
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::get(it, s.end()), "raytracer::parser::get", "Unexpected end of input");
}

Test(GetPeekExpect, peek_valid)
{
    const std::string s = "x";
    const auto it = s.begin();
    cr_assert_eq(raytracer::parser::peek(it, s.end()), 'x');
}

Test(GetPeekExpect, peek_empty)
{
    const std::string s;
    const auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::peek(it, s.end()), "raytracer::parser::peek", "Unexpected end of input");
}

Test(GetPeekExpect, expect_correct)
{
    const std::string s = "x";
    auto it = s.begin();
    raytracer::parser::expect(it, s.end(), 'x');
}

Test(GetPeekExpect, expect_wrong_char)
{
    const std::string s = "x";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::expect(it, s.end(), 'y'), "raytracer::parser::expect", "Expected 'y', got 'x'");
}

Test(GetPeekExpect, expect_empty)
{
    const std::string s;
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::expect(it, s.end(), 'x'), "raytracer::parser::get", "Unexpected end of input");
}

TestSuite(ParseBasic);

Test(ParseBasic, parse_null_success)
{
    const std::string s = "null";
    auto it = s.begin();
    const auto result = raytracer::parser::parseNull(it, s.end());
    cr_assert(std::holds_alternative<std::nullptr_t>(result));
}

Test(ParseBasic, parse_null_fail)
{
    const std::string s = "nul";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseNull(it, s.end()), "raytracer::parser::parseNull", "Expected 'null'");
}

Test(ParseBasic, parse_bool_true)
{
    const std::string s = "true";
    auto it = s.begin();
    const auto result = raytracer::parser::parseBool(it, s.end());
    cr_assert(std::holds_alternative<bool>(result));
    cr_assert(std::get<bool>(result));
}

Test(ParseBasic, parse_bool_false)
{
    const std::string s = "false";
    auto it = s.begin();
    const auto result = raytracer::parser::parseBool(it, s.end());
    cr_assert(std::holds_alternative<bool>(result));
    cr_assert(!std::get<bool>(result));
}

Test(ParseBasic, parse_bool_invalid)
{
    const std::string s = "tttt";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseBool(it, s.end()), "raytracer::parser::parseBool", "Expected 'true' or 'false'");
}

TestSuite(ParseNumber);

Test(ParseNumber, parse_integer)
{
    const std::string s = "42";
    auto it = s.begin();
    const auto result = raytracer::parser::parseNumber(it, s.end());
    cr_assert(std::holds_alternative<int>(result));
    cr_assert_eq(std::get<int>(result), 42);
}

Test(ParseNumber, parse_negative_float)
{
    const std::string s = "-3.14";
    auto it = s.begin();
    const auto result = raytracer::parser::parseNumber(it, s.end());
    cr_assert(std::holds_alternative<double>(result));
    cr_assert_float_eq(std::get<double>(result), -3.14, 0.0001);
}

Test(ParseNumber, parse_negative_integer)
{
    const std::string s = "-42";
    auto it = s.begin();
    const auto result = raytracer::parser::parseNumber(it, s.end());
    cr_assert(std::holds_alternative<int>(result));
    cr_assert_eq(std::get<int>(result), -42);
}

Test(ParseNumber, parse_float)
{
    const std::string s = "3.14";
    auto it = s.begin();
    const auto result = raytracer::parser::parseNumber(it, s.end());
    cr_assert(std::holds_alternative<double>(result));
    cr_assert_float_eq(std::get<double>(result), 3.14, 0.0001);
}

Test(ParseNumber, parse_invalid_number)
{
    const std::string s = ",aaa";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseNumber(it, s.end()), "raytracer::parser::parseNumber", "Invalid number");
}

Test(ParseNumber, parse_out_of_range_number)
{
    const std::string s = "99999999999999999999999999999999999999";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseNumber(it, s.end()), "raytracer::parser::parseNumber", "Invalid number");
}

TestSuite(ParseString);

Test(ParseString, simple_string)
{
    const std::string s = "\"hello\"";
    auto it = s.begin();
    const auto result = raytracer::parser::parseString(it, s.end(), false);
    cr_assert(std::holds_alternative<std::string>(result));
    cr_assert_str_eq(std::get<std::string>(result).c_str(), "hello");
}

Test(ParseString, string_with_escapes)
{
    const std::string s = R"("line\n\\\b\f\r\tend")";
    auto it = s.begin();
    const auto result = raytracer::parser::parseString(it, s.end(), false);
    cr_assert(std::holds_alternative<std::string>(result));
    cr_assert_str_eq(std::get<std::string>(result).c_str(), "line\n\\\b\f\r\tend");
}

Test(ParseString, unterminated_string)
{
    const std::string s = "\"unterminated";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseString(it, s.end(), false), "raytracer::parser::parseString", "Unterminated string");
}

Test(ParseString, invalid_escape)
{
    const std::string s = R"("bad\q")";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseString(it, s.end(), false), "raytracer::parser::parseString", "Invalid escape sequence");
}

TestSuite(ParseComplex);

Test(ParseComplex, parse_empty_array)
{
    const std::string s = "[]";
    auto it = s.begin();
    const auto result = raytracer::parser::parseArray(it, s.end());
    cr_assert(std::holds_alternative<std::nullptr_t>(result));
}

Test(ParseComplex, parse_array_of_numbers)
{
    const std::string s = "[1, 2,]";
    auto it = s.begin();
    const auto result = raytracer::parser::parseArray(it, s.end());
    const auto array = std::get<std::vector<raytracer::parser::JsonProto>>(result);
    cr_assert_eq(array.size(), 2);
}

Test(ParseComplex, parse_empty_object)
{
    const std::string s = "{}";
    auto it = s.begin();
    const auto result = raytracer::parser::parseObject(it, s.end());
    cr_assert(std::holds_alternative<std::nullptr_t>(result));
}

Test(ParseComplex, parse_simple_object)
{
    const std::string s = "{\"key\": 42,}";
    auto it = s.begin();
    const auto result = raytracer::parser::parseObject(it, s.end());
    const auto obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(result);
    cr_assert(obj.contains("key"));
}

Test(ParseComplex, parseObject_non_string_key)
{
    const std::string s = "{123: \"value\"}";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseObject(it, s.end()), "raytracer::parser::expect", "Expected '\"', got '1'");
}

Test(ParseComplex, parseObject_empty_key)
{
    const std::string s = R"({"": "value"})";
    auto it = s.begin();
    ASSERT_THROW_ERROR(raytracer::parser::parseObject(it, s.end()), "raytracer::parser::parseObject", "Expected string key");
}

TestSuite(ParseValue);

Test(ParseValue, parse_invalid_value)
{
    const std::string s = " ,1";
    auto it = s.begin();
    const auto end = s.end();

    ASSERT_THROW_ERROR(raytracer::parser::parseValue(it, end), "raytracer::parser::parseValue", "Unknown value");
}

Test(ParseValue, parse_negative_value)
{
    const std::string s = "-42";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<int>(result));
    cr_assert_eq(std::get<int>(result), -42);
}

Test(ParseValue, parse_negative_double_value)
{
    const std::string s = "-42.1";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<double>(result));
    cr_assert_eq(std::get<double>(result), -42.1);
}

Test(ParseValue, parse_value_null)
{
    const std::string s = "null";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<std::nullptr_t>(result));
    cr_assert_eq(std::get<std::nullptr_t>(result), nullptr);
}

Test(ParseValue, parse_true)
{
    const std::string s = "true";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<bool>(result));
    cr_assert_eq(std::get<bool>(result), true);
}

Test(ParseValue, parse_false)
{
    const std::string s = "false";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<bool>(result));
    cr_assert_eq(std::get<bool>(result), false);
}

Test(ParseValue, parse_array)
{
    const std::string s = "[1, 2, 3]";
    auto it = s.begin();
    const auto end = s.end();

    const auto result = raytracer::parser::parseValue(it, end);
    cr_assert(std::holds_alternative<std::vector<raytracer::parser::JsonProto>>(result));
    const auto vec = std::get<std::vector<raytracer::parser::JsonProto>>(result);
    cr_assert_eq(vec.size(), 3);
}

Test(ParseValue, parse_object)
{
    const std::string s = "{\"key\": 42,}";
    auto it = s.begin();
    const auto result = raytracer::parser::parseValue(it, s.end());
    const auto obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(result);
    cr_assert(obj.contains("key"));
}

TestSuite(FileTests);

Test(FileTests, parse_valid_json_file)
{
    const auto filename = "test_valid.json";
    std::ofstream file(filename);
    file << R"({"name": "Mathieu",})";
    file.close();

    auto result = raytracer::parser::parseJsonc(filename);
    auto obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(result);
    cr_assert(obj.contains("name"));

    std::remove(filename);
}

Test(FileTests, file_not_found)
{
    ASSERT_THROW_ERROR(raytracer::parser::parseJsonc("nonexistent.json"), "raytracer::parser::parseJsonc", "Could not open nonexistent.json");
}

Test(FileTests, file_with_trailing_data)
{
    const auto filename = "test_trailing.json";
    std::ofstream file(filename);
    file << R"({ "key": "value" }, trailing_data)";
    file.close();

    ASSERT_THROW_ERROR(raytracer::parser::parseJsonc(filename), "raytracer::parser::parseJsonc", "Unexpected trailing data in test_trailing.json");

    std::remove(filename);
}

TestSuite(check_comma);

Test(check_comma, valid_with_comma_and_value)
{
    const std::string input = " , 42";
    const auto it = input.begin();
    const auto end = input.end();

    cr_assert_no_throw(raytracer::parser::check_comma(it, end), raytracer::exception::Error);
}

Test(check_comma, valid_without_comma_before_closing)
{
    const std::string input = "  ]";
    const auto it = input.begin();
    const auto end = input.end();

    cr_assert_no_throw(raytracer::parser::check_comma(it, end), raytracer::exception::Error);
}

Test(check_comma, valid_without_comma_before_closing_object)
{
    const std::string input = "  }";
    const auto it = input.begin();
    const auto end = input.end();

    cr_assert_no_throw(raytracer::parser::check_comma(it, end), raytracer::exception::Error);
}

Test(check_comma, missing_comma_throws_error)
{
    const std::string input = " 42";
    const auto it = input.begin();
    const auto end = input.end();

    ASSERT_THROW_ERROR(raytracer::parser::check_comma(it, end), "raytracer::parser::check_comma", "Expected a comma");
}

Test(check_comma, trailing_comma_throws_error)
{
    const std::string input = " , ";
    const auto it = input.begin();
    const auto end = input.end();

    ASSERT_THROW_ERROR(raytracer::parser::check_comma(it, end), "raytracer::parser::check_comma", "Unexpected trailing comma");
}

Test(check_comma, test_wierd_case)
{
    const std::string input = "  " + std::string("") + "   ";
    const auto it = input.begin();
    const auto end = input.end();

    cr_assert_no_throw(raytracer::parser::check_comma(it, end), raytracer::exception::Error);
}

Test(check_comma, empty_input_no_throw)
{
    const std::string input = "   ";
    const auto it = input.begin();
    const auto end = input.end();

    cr_assert_no_throw(raytracer::parser::check_comma(it, end), raytracer::exception::Error);
}

/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_PrintParser
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Error.hpp"
#include "../src/Parser/PrintParser.hpp"

static void redirect_all_stdout()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(printJson, null_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = nullptr;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "null \"std::nullptr_t\"\n") != nullptr, "Missing null with debug type");
}

Test(printJson, string_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("hello");
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "hello \"std::string\"\n") != nullptr, "Missing string with debug type");
}

Test(printJson, int_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = 42;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "42 \"int\"\n") != nullptr, "Missing int with debug type");
}

Test(printJson, double_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = 3.14159;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "3.14159 \"double\"\n") != nullptr, "Missing double with debug type");
}

Test(printJson, bool_true_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = true;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "true \"bool\"\n") != nullptr, "Missing true bool with debug type");
}

Test(printJson, bool_false_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = false;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "false \"bool\"\n") != nullptr, "Missing false bool with debug type");
}

Test(printJson, array_of_ints_debug, .init=redirect_all_stdout)
{
    const std::vector array = {
        raytracer::parser::JsonProto(10),
        raytracer::parser::JsonProto(20),
        raytracer::parser::JsonProto(30)
    };
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[2048] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "[ \"array\"\n") != nullptr, "Missing opening array");
    cr_assert(strstr(buffer, "10 \"int\"\n") != nullptr, "Missing 10 int");
    cr_assert(strstr(buffer, "20 \"int\"\n") != nullptr, "Missing 20 int");
    cr_assert(strstr(buffer, "30 \"int\"\n") != nullptr, "Missing 30 int");
    cr_assert(strstr(buffer, "]\n") != nullptr, "Missing closing bracket for array");
}

Test(printJson, nested_object_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> inner_object = {
        {"nestedKey", raytracer::parser::JsonProto(std::string("nestedValue"))}
    };
    const std::unordered_map<std::string, raytracer::parser::JsonProto> outer_object = {
        {"outerKey", raytracer::parser::JsonProto(inner_object)}
    };
    const raytracer::parser::JsonValue val = outer_object;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[4096] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "{ \"object\"\n") != nullptr, "Missing opening object");
    cr_assert(strstr(buffer, "outerKey: { \"object\"\n") != nullptr, "Missing nested object");
    cr_assert(strstr(buffer, "nestedKey: nestedValue \"std::string\"\n") != nullptr, "Missing nested key-value");
    cr_assert(strstr(buffer, "}\n") != nullptr, "Missing closing bracket for object");
}

Test(printJson, null_no_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = nullptr;
    printJson(val, false);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret = fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "null\n") != nullptr, "Missing null without debug");
}

Test(printJson, string_special_chars, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("hello world! #$%");
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);


    cr_assert(strstr(buffer, "hello world! #$% \"std::string\"\n") != nullptr, "Missing special characters in string");
}

Test(printJson, empty_object, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {};
    const raytracer::parser::JsonValue val = object;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "{ \"object\"\n") != nullptr, "Missing opening object");
    cr_assert(strstr(buffer, "}\n") != nullptr, "Missing closing bracket for empty object");
}

Test(printJson, empty_array, .init=redirect_all_stdout)
{
    constexpr std::vector<raytracer::parser::JsonProto> array = {};
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "[ \"array\"\n") != nullptr, "Missing opening array");
    cr_assert(strstr(buffer, "]\n") != nullptr, "Missing closing bracket for empty array");
}

Test(printJson, mixed_array, .init=redirect_all_stdout)
{
    const std::vector array = {
        raytracer::parser::JsonProto(42),
        raytracer::parser::JsonProto(std::string("text")),
        raytracer::parser::JsonProto(true)
    };
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[2048] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "42 \"int\"\n") != nullptr, "Missing int in mixed array");
    cr_assert(strstr(buffer, "text \"std::string\"\n") != nullptr, "Missing string in mixed array");
    cr_assert(strstr(buffer, "true \"bool\"\n") != nullptr, "Missing bool in mixed array");
}

Test(printJson, object_with_array, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {
        {"arrayKey", raytracer::parser::JsonProto(std::vector{
            raytracer::parser::JsonProto(1),
            raytracer::parser::JsonProto(2)
        })}
    };
    const raytracer::parser::JsonValue val = object;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[4096] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "arrayKey: [ \"array\"\n") != nullptr, "Missing array inside object");
    cr_assert(strstr(buffer, "1 \"int\"\n") != nullptr, "Missing first int");
    cr_assert(strstr(buffer, "2 \"int\"\n") != nullptr, "Missing second int");
}

Test(printJson, array_with_object, .init=redirect_all_stdout)
{
    const std::vector array = {
        raytracer::parser::JsonProto(std::unordered_map<std::string, raytracer::parser::JsonProto>{
            {"key", raytracer::parser::JsonProto(123)}
        })
    };
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[4096] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "[ \"array\"\n") != nullptr, "Missing opening array");
    cr_assert(strstr(buffer, "{ \"object\"\n") != nullptr, "Missing object inside array");
    cr_assert(strstr(buffer, "key: 123 \"int\"\n") != nullptr, "Missing key-value inside object in array");
    cr_assert(strstr(buffer, "]\n") != nullptr, "Missing closing bracket for array");
}

Test(printJson, double_values, .init=redirect_all_stdout)
{
    const std::vector array = {
        raytracer::parser::JsonProto(0.0),
        raytracer::parser::JsonProto(-3.14),
    };
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[4096] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "0 \"double\"\n") != nullptr, "Missing 0 double");
    cr_assert(strstr(buffer, "-3.14 \"double\"\n") != nullptr, "Missing negative double");
}

Test(printJson, string_no_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("simple");
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "simple\n") != nullptr, "String without debug should just print value");
    cr_assert(strstr(buffer, "\"std::string\"") == nullptr, "Should not print type without debug");
}

Test(printJson, int_no_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = 1234;
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "1234\n") != nullptr, "Int without debug should just print number");
    cr_assert(strstr(buffer, "\"int\"") == nullptr, "Should not print type without debug");
}

Test(printJson, double_no_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = 3.1415;
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "3.1415\n") != nullptr, "Double without debug should just print number");
    cr_assert(strstr(buffer, "\"double\"") == nullptr, "Should not print type without debug");
}

Test(printJson, bool_true_no_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = true;
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert_str_eq(buffer, "true\n", "Bool true without debug should just be 'true\\n'");
}

Test(printJson, bool_false_no_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = false;
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert_str_eq(buffer, "false\n", "Bool false without debug should just be 'false\\n'");
}

Test(printJson, object_in_array_no_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::vector{
        raytracer::parser::JsonProto(std::unordered_map<std::string, raytracer::parser::JsonProto>{
            {"a", raytracer::parser::JsonProto(1)}
        })
    };
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);
    char buffer[4096] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "{\n") != nullptr, "Missing object inside array without debug");
    cr_assert(strstr(buffer, "a: 1\n") != nullptr, "Missing key:value pair inside object without debug");
}

Test(printJson, empty_array_debug, .init=redirect_all_stdout)
{
    constexpr std::vector<raytracer::parser::JsonProto> array = {};
    const raytracer::parser::JsonValue val = array;
    printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "[ \"array\"\n") != nullptr, "Missing array opening with type");
    cr_assert(strstr(buffer, "]\n") != nullptr, "Missing closing bracket for empty array");
}

Test(printJson, empty_array_no_debug, .init=redirect_all_stdout)
{
    constexpr std::vector<raytracer::parser::JsonProto> array = {};
    const raytracer::parser::JsonValue val = array;
    printJson(val, false);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "[\n") != nullptr, "Missing array opening with type");
    cr_assert(strstr(buffer, "]\n") != nullptr, "Missing closing bracket for empty array");
}

Test(printJson, double_with_debug, .init=redirect_all_stdout)
{
    constexpr raytracer::parser::JsonValue val = 42.42;
    printJson(val, true);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "42.42") != nullptr, "Should print the double value");
    cr_assert(strstr(buffer, "\"double\"") != nullptr, "Should also print type 'double' when debug is true");
}

Test(printJson, empty_object_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {};
    const raytracer::parser::JsonValue val = object;
    printJson(val, true);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "{ \"object\"\n") != nullptr, "Should open object with type");
    cr_assert(strstr(buffer, "}\n") != nullptr, "Should close empty object");
}

Test(printJson, empty_object_no_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {};
    const raytracer::parser::JsonValue val = object;
    printJson(val, false);

    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[512] = {};
    const std::size_t ret= fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);
    cr_assert_gt(ret, 1);

    cr_assert(strstr(buffer, "{\n") != nullptr, "Should open object");
    cr_assert(strstr(buffer, "}\n") != nullptr, "Should close empty object");
    cr_assert(strstr(buffer, "\"object\"") == nullptr, "Should not print type when debug is false");
}

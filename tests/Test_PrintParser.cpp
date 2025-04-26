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

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(printNull, print_without_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = nullptr;
    raytracer::parser::printNull(val, 2, false);
    cr_assert_stdout_eq_str("        null\n");
}

Test(printNull, print_with_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = nullptr;
    raytracer::parser::printNull(val, 1, true);
    cr_assert_stdout_eq_str("    null \"std::nullptr_t\"\n");
}

Test(printString, print_without_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("hello");
    raytracer::parser::printString(val, 0, false);
    cr_assert_stdout_eq_str("hello\n");
}

Test(printString, print_with_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("world");
    raytracer::parser::printString(val, 1, true);
    cr_assert_stdout_eq_str("    world \"std::string\"\n");
}

Test(printInt, print_without_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 42;
    raytracer::parser::printInt(val, 1, false);
    cr_assert_stdout_eq_str("    42\n");
}

Test(printInt, print_with_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = -1337;
    raytracer::parser::printInt(val, 0, true);
    cr_assert_stdout_eq_str("-1337 \"int\"\n");
}

Test(printDouble, print_without_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 3.14;
    raytracer::parser::printDouble(val, 2, false);
    cr_assert_stdout_eq_str("        3.14\n");
}

Test(printDouble, print_with_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = -0.5;
    raytracer::parser::printDouble(val, 0, true);
    cr_assert_stdout_eq_str("-0.5 \"double\"\n");
}

Test(printBool, print_true_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = true;
    raytracer::parser::printBool(val, 1, true);
    cr_assert_stdout_eq_str("    true \"bool\"\n");
}

Test(printBool, print_false_no_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = false;
    raytracer::parser::printBool(val, 0, false);
    cr_assert_stdout_eq_str("false\n");
}

Test(printBool, print_false_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = false;
    raytracer::parser::printBool(val, 1, true);
    cr_assert_stdout_eq_str("    false \"bool\"\n");
}

Test(printBool, print_true_no_debug, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = true;
    raytracer::parser::printBool(val, 0, false);
    cr_assert_stdout_eq_str("true\n");
}

Test(printArray, array_with_values, .init=redirect_all_stdout)
{
    const std::vector<raytracer::parser::JsonProto> array = {
        raytracer::parser::JsonProto(nullptr),
        raytracer::parser::JsonProto(123),
        raytracer::parser::JsonProto(4.5)
    };
    const raytracer::parser::JsonValue val = array;
    raytracer::parser::printArray(val, 0, false);
    cr_assert_stdout_eq_str(
        "[\n"
        "    null\n"
        "    123\n"
        "    4.5\n"
        "]\n"
    );
}

Test(printObject, object_with_values, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {
        {"key1", raytracer::parser::JsonProto(1)},
        {"key2", raytracer::parser::JsonProto(2)}
    };
    const raytracer::parser::JsonValue val = object;
    raytracer::parser::printObject(val, 0, false);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {0};
    (void)fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);

    cr_assert(strstr(buffer, "{\n") != NULL, "Missing opening object");
    cr_assert(strstr(buffer, "    1\n") != NULL, "Missing 1 int");
    cr_assert(strstr(buffer, "    2\n") != NULL, "Missing 2 int");
    cr_assert(strstr(buffer, "}\n") != NULL, "Missing closing bracket");
}

Test(printArray, array_with_values_debug, .init=redirect_all_stdout)
{
    const std::vector<raytracer::parser::JsonProto> array = {
        raytracer::parser::JsonProto(nullptr),
        raytracer::parser::JsonProto(123),
        raytracer::parser::JsonProto(4.5)
    };
    const raytracer::parser::JsonValue val = array;
    raytracer::parser::printArray(val, 0, true);
    cr_assert_stdout_eq_str(
        "[ \"array\"\n"
        "    null \"std::nullptr_t\"\n"
        "    123 \"int\"\n"
        "    4.5 \"double\"\n"
        "]\n"
    );
}

Test(printObject, object_with_values_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {
        {"key1", raytracer::parser::JsonProto(1)},
        {"key2", raytracer::parser::JsonProto(2)}
    };
    const raytracer::parser::JsonValue val = object;
    raytracer::parser::printObject(val, 0, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {0};
    (void)fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);

    cr_assert(strstr(buffer, "{ \"object\"\n") != NULL, "Missing opening object");
    cr_assert(strstr(buffer, "    1 \"int\"\n") != NULL, "Missing 1 int");
    cr_assert(strstr(buffer, "    2 \"int\"\n") != NULL, "Missing 2 int");
    cr_assert(strstr(buffer, "}\n") != NULL, "Missing closing bracket");
}

Test(printValue, null, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = nullptr;
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("null \"std::nullptr_t\"\n");
}

Test(printValue, string, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("nullptr");
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("nullptr \"std::string\"\n");
}

Test(printValue, intval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 100;
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("100 \"int\"\n");
}

Test(printValue, doubleval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 3.45;
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("3.45 \"double\"\n");
}

Test(printValue, falseval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = false;
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("false \"bool\"\n");
}

Test(printValue, trueval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = true;
    printValue(val, 0, true);
    cr_assert_stdout_eq_str("true \"bool\"\n");
}

Test(printValue, array_with_values_debug, .init=redirect_all_stdout)
{
    const std::vector<raytracer::parser::JsonProto> array = {
        raytracer::parser::JsonProto(nullptr),
        raytracer::parser::JsonProto(123),
        raytracer::parser::JsonProto(4.5)
    };
    const raytracer::parser::JsonValue val = array;
    raytracer::parser::printValue(val, 0, true);
    cr_assert_stdout_eq_str(
        "[ \"array\"\n"
        "    null \"std::nullptr_t\"\n"
        "    123 \"int\"\n"
        "    4.5 \"double\"\n"
        "]\n"
    );
}

Test(printValue, object_with_values_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {
        {"key1", raytracer::parser::JsonProto(1)},
        {"key2", raytracer::parser::JsonProto(2)}
    };
    const raytracer::parser::JsonValue val = object;
    raytracer::parser::printValue(val, 0, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {0};
    (void)fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);

    cr_assert(strstr(buffer, "{ \"object\"\n") != NULL, "Missing opening object");
    cr_assert(strstr(buffer, "    1 \"int\"\n") != NULL, "Missing 1 int");
    cr_assert(strstr(buffer, "    2 \"int\"\n") != NULL, "Missing 2 int");
    cr_assert(strstr(buffer, "}\n") != NULL, "Missing closing bracket");
}

Test(printJson, null, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = nullptr;
    printJson(val, true);
    cr_assert_stdout_eq_str("null \"std::nullptr_t\"\n");
}

Test(printJson, string, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = std::string("nullptr");
    printJson(val, true);
    cr_assert_stdout_eq_str("nullptr \"std::string\"\n");
}

Test(printJson, intval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 100;
    printJson(val, true);
    cr_assert_stdout_eq_str("100 \"int\"\n");
}

Test(printJson, doubleval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = 3.45;
    printJson(val, true);
    cr_assert_stdout_eq_str("3.45 \"double\"\n");
}

Test(printJson, falseval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = false;
    printJson(val, true);
    cr_assert_stdout_eq_str("false \"bool\"\n");
}

Test(printJson, trueval, .init=redirect_all_stdout)
{
    const raytracer::parser::JsonValue val = true;
    printJson(val, true);
    cr_assert_stdout_eq_str("true \"bool\"\n");
}

Test(printJson, array_with_values_debug, .init=redirect_all_stdout)
{
    const std::vector<raytracer::parser::JsonProto> array = {
        raytracer::parser::JsonProto(nullptr),
        raytracer::parser::JsonProto(123),
        raytracer::parser::JsonProto(4.5)
    };
    const raytracer::parser::JsonValue val = array;
    raytracer::parser::printJson(val, true);
    cr_assert_stdout_eq_str(
        "[ \"array\"\n"
        "    null \"std::nullptr_t\"\n"
        "    123 \"int\"\n"
        "    4.5 \"double\"\n"
        "]\n"
    );
}

Test(printJson, object_with_values_debug, .init=redirect_all_stdout)
{
    const std::unordered_map<std::string, raytracer::parser::JsonProto> object = {
        {"key1", raytracer::parser::JsonProto(1)},
        {"key2", raytracer::parser::JsonProto(2)}
    };
    const raytracer::parser::JsonValue val = object;
    raytracer::parser::printJson(val, true);
    FILE *stdout_file = cr_get_redirected_stdout();
    rewind(stdout_file);

    char buffer[1024] = {0};
    (void)fread(buffer, sizeof(char), sizeof(buffer) - 1, stdout_file);

    cr_assert(strstr(buffer, "{ \"object\"\n") != NULL, "Missing opening object");
    cr_assert(strstr(buffer, "    1 \"int\"\n") != NULL, "Missing 1 int");
    cr_assert(strstr(buffer, "    2 \"int\"\n") != NULL, "Missing 2 int");
    cr_assert(strstr(buffer, "}\n") != NULL, "Missing closing bracket");
}

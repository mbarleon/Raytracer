/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrintParser
*/

#include <iostream>
#include <ranges>
#include "PrintParser.hpp"

#if defined(UNIT_TESTS)
    namespace raytracer::parser {
#endif

// ReSharper disable once CppDFAConstantParameter
unit_static void printNull(__attribute__((unused))const raytracer::parser::JsonValue &val, const bool debug)
{
    std::cout <<  "null";
    if (debug) {
        std::cout << " \"" << "std::nullptr_t" << "\"";
    }
    std::cout << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printString(const raytracer::parser::JsonValue &val, const bool debug)
{
    const std::string str = std::get<std::string>(val);

    std::cout <<  str;
    if (debug) {
        std::cout << " \"" << "std::string" << "\"";
    }
    std::cout << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printInt(const raytracer::parser::JsonValue &val, const bool debug)
{
    const int i = std::get<int>(val);

    std::cout <<  i;
    if (debug) {
        std::cout << " \"" << "int" << "\"";
    }
    std::cout << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printDouble(const raytracer::parser::JsonValue &val, const bool debug)
{
    const double d = std::get<double>(val);

    std::cout <<  d;
    if (debug) {
        std::cout << " \"" << "double" << "\"";
    }
    std::cout << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printBool(const raytracer::parser::JsonValue &val, const bool debug)
{
    const bool b = std::get<bool>(val);

    std::cout <<  (b ? "true" : "false");
    if (debug) {
        std::cout << " \"" << "bool" << "\"";
    }
    std::cout << std::endl;
}

unit_static void printValue(const raytracer::parser::JsonValue &val, std::size_t indentation_level, bool debug);

// ReSharper disable once CppDFAConstantParameter
unit_static void printArray(const raytracer::parser::JsonValue &val, const std::size_t indentation_level, const bool debug) // NOLINT(*-no-recursion)
{
    std::cout << '[';
    if (debug) {
        std::cout << " \"" << "array" << "\"";
    }
    std::cout << std::endl;

    for (const auto arr = std::get<std::vector<raytracer::parser::JsonProto>>(val); const auto &elem : arr) {
        std::cout << std::string((indentation_level + 1) * 4, ' ');
        printValue(elem.value, indentation_level + 1, debug);
    }

    std::cout << std::string(indentation_level * 4, ' ') << ']' << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printObject(const raytracer::parser::JsonValue &val, const std::size_t indentation_level, const bool debug) // NOLINT(*-no-recursion)
{
    std::cout << '{';
    if (debug) {
        std::cout << " \"" << "object" << "\"";
    }
    std::cout << std::endl;

    for (const auto &obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(val);
        const auto &[fst, snd] : obj) {
        std::cout << std::string((indentation_level + 1) * 4, ' ') << fst << ": ";
        printValue(snd.value, indentation_level + 1, debug);
    }

    std::cout << std::string(indentation_level * 4, ' ') << '}' << std::endl;
}

// ReSharper disable once CppDFAConstantParameter
unit_static void printValue(const raytracer::parser::JsonValue &val, const std::size_t indentation_level, const bool debug) // NOLINT(*-no-recursion)
{
    if (std::holds_alternative<std::nullptr_t>(val)) {
        printNull(val, debug);
    } else if (std::holds_alternative<std::string>(val)) {
        printString(val, debug);
    } else if (std::holds_alternative<int>(val)) {
        printInt(val, debug);
    } else if (std::holds_alternative<double>(val)) {
        printDouble(val, debug);
    } else if (std::holds_alternative<bool>(val)) {
        printBool(val, debug);
    } else if (std::holds_alternative<std::vector<raytracer::parser::JsonProto>>(val)) {
        printArray(val, indentation_level, debug);
    } else if (std::holds_alternative<std::unordered_map<std::string, raytracer::parser::JsonProto>>(val)) {
        printObject(val, indentation_level, debug);
    }
}

#if defined(UNIT_TESTS)
    };//namespace raytracer::parser
#endif

void raytracer::parser::printJsonc(const JsonValue &val, const bool debug = false)
{
    printValue(val, 0, debug);
}

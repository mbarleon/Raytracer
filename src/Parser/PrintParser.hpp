/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrintParser
*/

#pragma once

#include "Macro.hpp"
#include "ParserTypes.hpp"

namespace raytracer::parser {

void printJson(const JsonValue &val, bool debug);

#if defined(UNIT_TESTS)
    unit_static void printNull(__attribute__((unused))const JsonValue &val, std::size_t indentation_level, const bool debug);
    unit_static void printString(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printInt(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printDouble(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printBool(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printArray(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printObject(const JsonValue &val, const std::size_t indentation_level, const bool debug);
    unit_static void printValue(const JsonValue &val, const std::size_t indentation_level, const bool debug);
#endif

}//namespace raytracer::parser

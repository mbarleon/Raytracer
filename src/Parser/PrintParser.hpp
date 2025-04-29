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

void printJsonc(const JsonValue &val, bool debug);

#if defined(UNIT_TESTS)
    unit_static void printNull(__attribute__((unused))JsonValue &val, bool debug);
    unit_static void printString(JsonValue &val, bool debug);
    unit_static void printInt(JsonValue &val, bool debug);
    unit_static void printDouble(JsonValue &val, bool debug);
    unit_static void printBool(JsonValue &val, bool debug);
    unit_static void printArray(JsonValue &val, std::size_t indentation_level, bool debug);
    unit_static void printObject(JsonValue &val, std::size_t indentation_level, bool debug);
    unit_static void printValue(JsonValue &val, std::size_t indentation_level, bool debug);
#endif

}//namespace raytracer::parser

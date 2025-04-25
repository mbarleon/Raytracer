/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Parser
*/

#include "Parser.hpp"

raytracer::parser::JsonValue raytracer::parser::parseJson(const char * RESTRICT filepath)
{
    JsonValue ret = nullptr;
    std::ifstream file(filepath);

    if (!file.is_open())
        throw exception::Error("raytracer::parser::parseJson", "Failed to open ", filepath);

    return ret;
}

/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include "../Parser/Parser.hpp"
#include "../Parser/PrintParser.hpp"
#include "Logger.hpp"
#include <iostream>
#include <unordered_map>

/*
* public
*/

/**
 * @brief Core run
 * @details Core entry-point, starts the raytracer program
 * @return void
 */
void raytracer::Core::run(const char *RESTRICT filename)
{
    const auto jsonc = parser::parseJson(filename);
    const auto a = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(jsonc);

    (void) a;
}

/*
* private
*/

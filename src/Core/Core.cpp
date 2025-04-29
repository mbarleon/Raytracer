/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include "../Parser/Parser.hpp"
#include "CoreFactory.hpp"
#include "Logger.hpp"
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
    const parser::JsonValue jsonc = parser::parseJson(filename);
    const auto &root = std::get<std::unordered_map<std::string, parser::JsonProto>>(jsonc);
    const auto &scene = std::get<std::unordered_map<std::string, parser::JsonProto>>(root.at("scene").value);
    const auto &primitives = scene.at("primitives");
    const auto spheres = get_spheres(primitives);

    logger::debug("Core loaded: ", spheres.size(), " sphere(s) from ", filename);
}

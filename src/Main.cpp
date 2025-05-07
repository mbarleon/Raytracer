/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main.cpp
*/

#include "Core/Core.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include "Parser/ParseArguments.hpp"

int main(const int argc, char **argv)
{
    try {
        if (raytracer::parser::parse_arguments(argc, argv)) {
            raytracer::Core core;
            core.run(argv[1]);
        }
    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
        return ERROR;
    }
    return SUCCESS;
}

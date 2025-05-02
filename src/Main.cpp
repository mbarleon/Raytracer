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
#include <cstring>

int help()
{
    std::cout << "USAGE: ./raytracer <SCENE_FILE>" << std::endl
        << "    SCENE_FILE: scene configuration" << std::endl;
    return SUCCESS;
}

int main(const int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "-help") == 0)
        return help();
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

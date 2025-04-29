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
#include "Parser/Parser.hpp"
#include "Parser/PrintParser.hpp"

int main(const int argc, char **argv)
{
    try {
        if (raytracer::parser::parse_arguments(argc, argv)) {
            const auto json = raytracer::parser::parseJson(argv[1]);

            printJson(json, true);
            raytracer::Core core;
            core.run();
        }

    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
        return ERROR;
    }
    return SUCCESS;
}

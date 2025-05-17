/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main.cpp
*/

#include "Core/Application.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include "Parser/ParseArguments.hpp"

int main(const int argc, const char **argv)
{
    try {
        const int parseResult = raytracer::parser::parse_arguments(argc, argv);

        if (parseResult != 0) {
            raytracer::core::Application app(argv[1], parseResult == 1);

            if (parseResult == 1) {
                app.run();
            } else {
                app.runNoGUI();
            }
        }
    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
        return ERROR;
    }
    return SUCCESS;
}

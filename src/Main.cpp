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

int main(const int argc, char **argv)
{
    try {
        if (raytracer::parser::parse_arguments(argc, argv)) {
            raytracer::core::Application app;

            app.run();
        }
    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
        return ERROR;
    }
    return SUCCESS;
}

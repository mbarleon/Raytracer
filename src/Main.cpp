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

int main(const int argc, char **argv)
{
    (void) argc;
    (void) argv;
    try {
        raytracer::Core core;

        core.run();
    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
        return ERROR;
    }
    return SUCCESS;
}

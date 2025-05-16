/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ParseArguments.cpp
*/

#include "ParseArguments.hpp"
#include "Error.hpp"
#include <cstring>
#include <iostream>
#include <sys/stat.h>

#if defined(UNIT_TESTS)
namespace raytracer::parser {
#endif

unit_static bool filepath_exists(const char *RESTRICT filepath)
{
    struct stat st;

    return stat(filepath, &st) == 0;
}

unit_static void version()
{
    std::cout << R"(
  ╭──────────────────────────────────────╮
  │        Raytracer — Version 0.0.1     │
  ╰──────────────────────────────────────╯
)" << std::endl;
}

unit_static void usage()
{
    std::cout << R"(
USAGE:
    ./raytracer <SCENE_FILE.jsonc>

ARGUMENTS:
    SCENE_FILE.jsonc   Path to the scene configuration file (JSON with comments)

EXAMPLES:
    ./raytracer scenes/simple_scene.jsonc

)" << std::endl;
}

#if defined(UNIT_TESTS)
}
#endif

bool raytracer::parser::parse_arguments(const int argc, const char **argv)
{
    if (argc != 2) {
        throw exception::Error("parser::parse_arguments", "invalid number of arguments");
    }
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        version();
        return false;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        usage();
        return false;
    }
    if (!filepath_exists(argv[1])) {
        throw exception::Error("parser::parse_arguments", "invalid config file: ", argv[1], " file not found.");
    }
    return true;
}

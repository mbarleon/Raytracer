/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ParseArguments.hpp
*/

#pragma once

#include "Macro.hpp"

namespace raytracer::parser {

int parse_arguments(const int argc, const char **argv);

#if defined(UNIT_TESTS)
unit_static bool filepath_exists(const char *RESTRICT filepath);
unit_static void version(void);
unit_static void usage(void);
#endif

}// namespace raytracer::parser

/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ParseArguments.hpp
*/

#pragma once

namespace raytracer::parser {

[[nodiscard]] bool parse_arguments(int argc, char **argv);

#if defined(UNIT_TESTS)
unit_static void filepath_exists(const char *RESTRICT filepath);
unit_static void version(void);
unit_static void usage(void);
#endif

}// namespace raytracer::parser

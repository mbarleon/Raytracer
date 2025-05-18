/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_ParseArguments
*/

#include "../src/Parser/ParseArguments.hpp"
#include "Error.hpp"
#include <criterion/criterion.h>
#include <iostream>

Test(parse_arguments, test_parse_arguments_valid)
{
    const char *argv[2] = {"raytracer", "../flake.nix"};
    const bool result = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv));

    cr_assert_eq(result, true);
}

Test(parser_arguments, test_parse_arguments_invalid_argc)
{
    try {
        const bool __attribute__((unused)) rv = raytracer::parser::parse_arguments(1, nullptr);
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "invalid number of arguments");
    }
}

Test(parse_arguments, test_parse_arguments_invalid_filename)
{
    try {
        const char *argv[2] = {"raytracer", "flake.nixxxxx"};
        const bool __attribute__((unused)) rv = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv));
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "invalid config file: flake.nixxxxx file not found.");
    }
}

Test(parse_arguments, test_parse_arguments_version)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    const char *argv1[2] = {"raytracer", "-v"};
    const char *argv2[2] = {"raytracer", "--version"};
    const bool rv1 = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv1));
    const bool rv2 = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv2));

    std::cout.rdbuf(old);
    cr_assert_eq(rv1, false);
    cr_assert_eq(rv2, false);
}

Test(parse_arguments, test_parse_arguments_usage)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    const char *argv1[2] = {"raytracer", "-h"};
    const char *argv2[2] = {"raytracer", "--help"};
    const bool rv1 = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv1));
    const bool rv2 = raytracer::parser::parse_arguments(2, const_cast<const char **>(argv2));

    std::cout.rdbuf(old);
    cr_assert_eq(rv1, false);
    cr_assert_eq(rv2, false);
}

Test(usage, test_usage)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    raytracer::parser::usage();

    std::cout.rdbuf(old);
    cr_assert(buffer.str().find("raytracer") != std::string::npos);
}

Test(version, test_version)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    raytracer::parser::version();

    std::cout.rdbuf(old);
    cr_assert(buffer.str().find("Raytracer") != std::string::npos);
}

Test(filepath_exists, test_filepath)
{
    const bool rv1 = raytracer::parser::filepath_exists("../flake.nix");
    const bool rv2 = raytracer::parser::filepath_exists("flake.nixx");

    cr_assert_eq(rv1, true);
    cr_assert_eq(rv2, false);
}

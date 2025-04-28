/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Logger.cpp
*/

#include "Logger.hpp"
#include <criterion/criterion.h>
#include <iostream>

Test(debug, test_logger_debug)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cerr.rdbuf(buffer.rdbuf());

    try {
        throw raytracer::exception::Error("wrong", "caca");
    } catch (const raytracer::exception::Error &e) {
        raytracer::logger::error(e);
    }

    std::cerr.rdbuf(old);

    cr_assert(buffer.str().find("wrong") != std::string::npos);
}

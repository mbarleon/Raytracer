/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Logger.cpp
*/

#include "Logger.hpp"
#include <criterion/criterion.h>
#include <iostream>
#include <sstream>
#include <streambuf>

Test(error, test_logger_error)
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

Test(progress_bar, test_progress_bar)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cerr.rdbuf(buffer.rdbuf());

    const float y = 100.f;

    for (float ny = 0.f; ny < y; ++ny) {
        raytracer::logger::progress_bar(y, ny);
    }
    raytracer::logger::progress_bar(y, y);
    std::cerr.rdbuf(old);
    cr_assert(buffer.str().find("[") != std::string::npos);
}

Test(progress_bar, test_progress_bar_failed)
{
    const std::stringstream buffer;
    std::streambuf *old = std::cerr.rdbuf(buffer.rdbuf());

    const float y = 0.f;

    for (float ny = 0.f; ny < y; ++ny) {
        raytracer::logger::progress_bar(y, ny);
    }
    raytracer::logger::progress_bar(y, y);
    std::cerr.rdbuf(old);
    cr_assert(buffer.str().empty() == true);
}

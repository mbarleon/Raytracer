/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Logger.cpp
*/

#include "Logger.hpp"
#include <iostream>

/**
 * @brief logger::error
 * @details takes an exception::Error and display clearly what is
 * the Error, where was it raised and why
 * @return void
 */
void raytracer::logger::error(const exception::Error &e)
{
    std::cerr << RED_BOLD << "╔════════════════════════════════╗" << RESET << std::endl
              << RED_BOLD << "║       ⚠ ERROR OCCURRED ⚠       ║" << RESET << std::endl
              << RED_BOLD << "╚════════════════════════════════╝" << RESET << std::endl
              << YELLOW << "⮞ Raised by: " << RESET << e.where() << std::endl
              << YELLOW << "⮞ Reason:    " << RESET << e.what() << std::endl;
}

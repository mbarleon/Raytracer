/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Logger.cpp
*/

#include "Logger.hpp"
#include <iomanip>
#include <iostream>

/**
 * @brief logger::error
 * @details takes an exception::Error and display clearly what is
 * the Error, where was it raised and why
* @param e the exception::Error to display
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

/**
* @brief logger::progress_bar
* @details displays a progress bar in the console
* @param total_wait the total number of steps
* @param current_state the current step
* @return void
*/
void raytracer::logger::progress_bar(const float total_wait, const float current_state)
{
    if (total_wait == 0 || current_state > total_wait) {
        return;
    }

    const float progress = static_cast<float>(current_state) / static_cast<float>(total_wait);
    const float filled = static_cast<float>(progress * PROGRESS_BAR_WIDTH);
    const float percent = progress * HUNDRED_PERCENT;

    std::cerr << "\r[";
    for (uint32 i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        std::cerr << (i < static_cast<uint32>(filled) ? "━" : " ");
    }
    std::cerr << "] " << std::setw(PROGRESS_BAR_SET_WIDTH) << static_cast<int>(percent) << "%";

    if (current_state == total_wait) {
        std::cerr << std::endl;
    }
}

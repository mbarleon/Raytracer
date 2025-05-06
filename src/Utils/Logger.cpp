/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Logger.cpp
*/

#define PROGRESS_BAR_ENABLE
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
    if (total_wait <= 0.0f || current_state > total_wait) {
        return;
    }

    static bool cursor_hidden = false;
    const float progress = current_state / total_wait;
    const uint32 filled_length = static_cast<uint32>(progress * PROGRESS_BAR_WIDTH);
    const uint32 percent = static_cast<uint32>(progress * HUNDRED_PERCENT);
    const uint32 segment_size = PROGRESS_BAR_WIDTH / NUM_COLORS;

    if (!cursor_hidden) {
        std::cerr << "\x1b[?25l";
        cursor_hidden = true;
    }

    std::cerr << "\r[";

    for (uint32 i = 0; i < PROGRESS_BAR_WIDTH; ++i) {

        if (i < filled_length) {
            uint32 idx = i / segment_size;

            if (idx >= NUM_COLORS) {
                idx = NUM_COLORS - 1;
            }

            std::cerr << colored_blocks[idx];

        } else {
            std::cerr << ' ';
        }
    }

    std::cerr << "] " << std::setw(PROGRESS_BAR_SET_WIDTH) << percent << "%";

    if (current_state == total_wait) {
        std::cerr << "\x1b[?25h\n";
        cursor_hidden = false;
    } else {
        std::cerr << std::flush;
    }
}

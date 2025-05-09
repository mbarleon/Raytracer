/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Vector2u.cpp
*/

#include "../src/Maths/Vector2u.hpp"
#include <criterion/criterion.h>
#include <iostream>

Test(test_vector2u, test_vector2u_valid)
{
    const math::Vector2u vec2 = {
        .x = 24,
        .y = 53,
    };

    cr_assert_eq(vec2.x, 24);
    cr_assert_eq(vec2.y, 53);
}

Test(test_vector2u_print, test_vector2u_valid_print_operator)
{
    const math::Vector2u vec2 = {
        .x = 24,
        .y = 53,
    };
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    std::cout << vec2 << std::endl;
    std::cout.rdbuf(old);
    cr_assert(buffer.str().find("24") != std::string::npos);
    cr_assert(buffer.str().find("53") != std::string::npos);
}

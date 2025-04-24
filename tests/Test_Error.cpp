/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Error.cpp
*/

#include "Error.hpp"
#include <criterion/criterion.h>

Test(error, test_error)
{
    try {
        throw raytracer::exception::Error("FONCTION", "MARCHE_PAS");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "FONCTION");
        cr_assert_str_eq(e.what(), "MARCHE_PAS");
    }
}

Test(error, test_error_long)
{
    try {
        throw raytracer::exception::Error("FONCTION", "MARCHE_PAS", " DU TOUT", 42);
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.where(), "FONCTION");
        cr_assert_str_eq(e.what(), "MARCHE_PAS DU TOUT42");
    }
}

/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main.cpp
*/

#include "Error.hpp"

int main(void)
{
    throw raytracer::exception::Error("main", "hello world");
}

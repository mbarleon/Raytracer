/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main.cpp
*/

#include "Error.hpp"
#include "Logger.hpp"

int main(void)
{
    try {
        throw raytracer::exception::Error("main", "hello world");
    } catch (raytracer::exception::Error &e) {
        raytracer::logger::error(e);
    }
}

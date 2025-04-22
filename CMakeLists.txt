##
## EPITECH PROJECT, 2025
## Raytracer
## File description:
## Main.cpp
##

cmake_minimum_required(VERSION 3.16)
project(Raytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_compile_options(-Wall -Wextra -Werror -pedantic -Wconversion -g3)

########################################

file(GLOB UTILS_SOURCE "src/Utils/*.cpp")

########################################

file(GLOB CORE_SOURCE
    "src/*.cpp"
    "src/Core/*.cpp"
)

add_executable(raytracer ${CORE_SOURCE} ${UTILS_SOURCE})
########################################

########################################

##
## EPITECH PROJECT, 2025
## Raytracer
## File description:
## CMakeList.txt
##

cmake_minimum_required(VERSION 3.16)
project(Raytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_compile_options(-Wall -Wextra -Werror -pedantic -Wconversion)

########################################

file(GLOB UTILS_SOURCE "src/Utils/*.cpp")

########################################

file (GLOB MAIN_SOURCE
    "src/Main.cpp")

file(GLOB MATH_SOURCE
    "src/Maths/*.cpp")

file(GLOB CORE_SOURCE
    "src/Core/*.cpp")

file (GLOB PARSER_SOURCE
    "src/Parser/*.cpp")

add_executable(raytracer ${CORE_SOURCE} ${MATH_SOURCE} ${UTILS_SOURCE} ${PARSER_SOURCE} ${MAIN_SOURCE})

########################################

target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/include)

########################################

file (GLOB SHAPE_SOURCE
    "src/Shapes/*.cpp")

########################################

file(GLOB TEST_SOURCE
    "tests/*.cpp")

add_executable(unit_tests ${CORE_SOURCE} ${MATH_SOURCE} ${SHAPE_SOURCE} ${UTILS_SOURCE} ${PARSER_SOURCE} ${TEST_SOURCE})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    target_compile_options(unit_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(unit_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(unit_tests PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(unit_tests PRIVATE -fprofile-arcs -ftest-coverage)
endif()

target_compile_definitions(unit_tests PRIVATE UNIT_TESTS=1)

target_link_libraries(unit_tests PRIVATE criterion)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)

add_custom_target(tests_run
    COMMAND unit_tests
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

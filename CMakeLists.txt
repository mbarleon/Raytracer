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
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

########################################
# INFO: enable multithreading for build
########################################
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

########################################
# INFO: CMake cache management for faster builds
########################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

########################################
# INFO: SFML configuration
########################################
add_subdirectory(external/SFML EXCLUDE_FROM_ALL)

########################################

file(GLOB UTILS_SOURCE "src/Utils/*.cpp")

#TODO: in the future, .so for shapes in a folder ./plugins/
#INFO: Shapes are currently compiled with Core

########################################

file (GLOB MAIN_SOURCE
    "src/Main.cpp")

file(GLOB MATH_SOURCE
    "src/Maths/*.cpp")

file(GLOB CORE_SOURCE
    "src/Core/*.cpp")

file (GLOB PARSER_SOURCE
    "src/Parser/*.cpp")

file(GLOB_RECURSE SCENE_SOURCE
    "src/Elements/*.cpp")

file(GLOB_RECURSE UI_SOURCE
    "src/UI/*.cpp")


add_executable(raytracer
    ${UTILS_SOURCE}
    ${MATH_SOURCE}
    ${SCENE_SOURCE}
    ${PARSER_SOURCE}
    ${UI_SOURCE}
    ${CORE_SOURCE}
    ${MAIN_SOURCE}
)

# NOTE: commented because the project wont compile anymore lol
# target_compile_options(raytracer PRIVATE
#     # INFO: classic yutsu warnings
#     -Wall
#     -Wextra
#     -Werror
#     -pedantic
#
#     # INFO: type safety & conversions
#     -Wconversion
#     -Wsign-conversion
#     -Wfloat-equal
#
#     # INFO: code correctness
#     -Wshadow
#     -Wnull-dereference
#     -Wundef
#     -Wuninitialized
#     -Winit-self
#     -Wredundant-decls
#
#     # INFO: code hygiene
#     -Wcast-align
#     -Wcast-qual
#     -Wmissing-declarations
#     -Woverloaded-virtual
#     -Wnon-virtual-dtor
#
#     # INFO: logic bugs & bad practices
#     -Wduplicated-cond
#     -Wduplicated-branches
#     -Wlogical-op
#     -Wswitch-default
#     -Wswitch-enum
#     -Wuseless-cast
#
#     # INFO: prevent common portability issues
#     -Wdouble-promotion
#     -Wformat=2
#     -Wwrite-strings
# )

########################################

option(ENABLE_DEBUG "Enable debug definition" OFF)

if(ENABLE_DEBUG)
    target_compile_definitions(raytracer PRIVATE DEBUG=1)
endif()

target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(raytracer PRIVATE sfml-graphics sfml-system sfml-window)

########################################

file(GLOB TEST_SOURCE
    "tests/*.cpp")

add_executable(unit_tests
    ${UTILS_SOURCE}
    ${MATH_SOURCE}
    ${SCENE_SOURCE}
    ${PARSER_SOURCE}
    ${UI_SOURCE}
    ${CORE_SOURCE}
    ${TEST_SOURCE}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    target_compile_options(unit_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(unit_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(unit_tests PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(unit_tests PRIVATE -fprofile-arcs -ftest-coverage)
endif()

target_compile_definitions(unit_tests PRIVATE UNIT_TESTS=1)

target_link_libraries(unit_tests PRIVATE criterion sfml-graphics sfml-system sfml-window)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)

add_custom_target(tests_run
    COMMAND unit_tests
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

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
# INFO: Define source files
########################################

file(GLOB UTILS_SOURCE "src/Utils/*.cpp")

file(GLOB MAIN_SOURCE
    "src/Main.cpp")

file(GLOB MATH_SOURCE
    "src/Maths/*.cpp")

file(GLOB CORE_SOURCE
    "src/Core/*.cpp")

file(GLOB PARSER_SOURCE
    "src/Parser/*.cpp")

file(GLOB_RECURSE SCENE_SOURCE
    "src/Elements/*.cpp")

file(GLOB_RECURSE UI_SOURCE
    "src/UI/*.cpp")

########################################
# INFO: Find installed SFML or use the external one
########################################

option(USE_EXTERNAL_SFML "Use SFML from external directory" OFF)

# by default try to find pre-installed SFML
if(NOT USE_EXTERNAL_SFML)
    find_package(SFML 2.6.4 COMPONENTS graphics window system QUIET)
endif()

# if we explicitly want to use external SFML or couldn't find it
if(USE_EXTERNAL_SFML OR NOT SFML_FOUND)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples")
    set(SFML_BUILD_DOC OFF CACHE BOOL "Build SFML documentation")
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio module")
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML network module")
    add_subdirectory(external/SFML EXCLUDE_FROM_ALL)
endif()

########################################
# INFO: Define raytracer target
########################################

add_executable(raytracer
    ${UTILS_SOURCE}
    ${MATH_SOURCE}
    ${SCENE_SOURCE}
    ${PARSER_SOURCE}
    ${UI_SOURCE}
    ${CORE_SOURCE}
    ${MAIN_SOURCE}
)

target_compile_options(raytracer PRIVATE
    # INFO: classic yutsu warnings
    -Wall
    -Wextra
    -Werror
    -pedantic

    # INFO: type safety & conversions
    -Wconversion
    -Wsign-conversion
    # -Wfloat-equal # << commented because this flag is PURE HATE

    # INFO: code correctness
    -Wshadow
    -Wnull-dereference
    -Wundef
    -Wuninitialized
    -Winit-self
    -Wredundant-decls

    # INFO: code hygiene
    -Wcast-align
    -Wcast-qual
    -Wmissing-declarations
    -Woverloaded-virtual
    -Wnon-virtual-dtor

    # INFO: logic bugs & bad practices
    # -Wduplicated-cond << not on clang
    # -Wduplicated-branches << not on clang
    # -Wlogical-op << not on clang
    -Wswitch-default
    # -Wswitch-enum << commented because of SFML enums are toooooooo big
    # -Wuseless-cast << not on clang

    # INFO: prevent common portability issues
    # -Wdouble-promotion << commented because of STL.cpp & Utils.cpp wont compile
    -Wformat=2
    -Wwrite-strings
)

########################################
# INFO: Debug mode
########################################

option(ENABLE_DEBUG "Enable debug definition" OFF)

if(ENABLE_DEBUG)
    target_compile_definitions(raytracer PRIVATE DEBUG=1)
endif()

target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(raytracer PRIVATE sfml-graphics sfml-system sfml-window)

########################################
# INFO: Custom targets
########################################

add_custom_target(build_sfml
    COMMENT "Building external SFML"
    COMMAND ${CMAKE_COMMAND} -DUSE_EXTERNAL_SFML=ON ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target sfml-system
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target sfml-window
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target sfml-graphics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(external DEPENDS build_sfml)

########################################
# INFO: Testing configuration
########################################

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
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
endif()

add_custom_target(tests_run
    COMMENT "Running unit tests"
    COMMAND ${CMAKE_COMMAND} -DBUILD_TESTS=ON ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target unit_tests
    COMMAND ${CMAKE_BINARY_DIR}/unit_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

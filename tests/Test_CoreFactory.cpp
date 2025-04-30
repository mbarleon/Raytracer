/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_CoreFactory.cpp
*/

#include "../src/Core/CoreFactory.hpp"
#include "../src/Parser/Parser.hpp"
#include <criterion/criterion.h>

using JsonMap = std::unordered_map<std::string, raytracer::parser::JsonProto>;

Test(create_camera, test_primitive_factory_camera)
{
    const std::string input =
        "{\"scene\": {\"camera\": {\"resolution\": {\"width\": 1920,\"height\": 1080, },\"position\": {\"x\": 0,\"y\": -100,\"z\": 20, },\"rotation\": {\"x\": 0,\"y\": 0,\"z\": 0, },\"fieldOfView\": 70, }, }, }";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &camera = scene.at("camera");
    const std::unique_ptr<raytracer::Camera> camera_ptr = create_camera(camera);

    cr_assert_not_null(camera_ptr);
}

Test(primitive_factory, test_primitive_factory_camera)
{
    const std::string input =
        "{\"scene\": {\"primitives\": {\"spheres\": [ {\"position\": {\"x\": 60.03,\"y\": 5,\"z\": 40, },\"radius\": 25,\"material\": {\"type\":\"solid\",\"color\": {\"r\": 255,\"g\": 64,\"b\": 64, },\"reflectivity\": 0.2,\"transparency\": 0,\"refractiveIndex\": 1, },\"transformations\": [ {\"type\":\"translation\",\"x\": 0,\"y\": 0,\"z\": 0, }, ], }, ],\"rectangles\": [ {\"origin\": {\"x\": 60,\"y\": 5,\"z\": 40, },\"bottom_side\": {\"x\": 10,\"y\": 20,\"z\": 30, },\"left_side\": {\"x\": 40,\"y\": 50,\"z\": 60, },\"material\": {\"type\":\"solid\",\"color\": {\"r\": 255,\"g\": 64,\"b\": 64, },\"reflectivity\": 0.2,\"transparency\": 0,\"refractiveIndex\": 1, }, }, ], }, }, }";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &primitives = scene.at("primitives");
    const IShapesList &primitives_ptr = primitive_factory(primitives);

    cr_assert(primitives_ptr.begin() != primitives_ptr.end());
}

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
        "{\"scene\": {\"camera\": {\"resolution\": {\"width\": 1920,\"height\": 1080, },\"position\": {\"x\": 0,\"y\": -100,\"z\": 20, },\"rotation\": {\"x\": 0,\"y\": 0,\"z\": 0, },\"fov\": 70, }, }, }";

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
        "{ \"scene\": { \"camera\": { \"resolution\": { \"width\": 1920, \"height\": 1080 }, \"position\": { \"x\": 0, \"y\": 0, \"z\": 0 }, \"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 }, \"fov\": 70 },\"render\": { \"background-color\": { \"r\": 135, \"g\": 206, \"b\": 250 }, \"antialiasing\": { \"type\": \"supersampling\", \"samples\": 4}, \"output\": { \"file\": \"output.ppm\", \"format\": \"ppm\" }, \"max-depth\": 5 }, \"materials\": [ { \"name\": \"corail-red\", \"color\": { \"r\": 255, \"g\": 64, \"b\": 64 }, \"reflectivity\": 0.2,\"transparency\": 0, \"refractive-index\": 1 } ], \"primitives\": { \"spheres\": [ {\"position\": { \"x\": 0, \"y\": 0, \"z\": -6 }, \"radius\": 2, \"material\": \"corail-red\" } ],}}}";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &primitives = scene.at("primitives");
    const MaterialsList &materials = material_factory(root.at("scene"));
    const IShapesList &primitives_ptr = primitive_factory(primitives, materials);

    cr_assert(primitives_ptr.begin() != primitives_ptr.end());
}

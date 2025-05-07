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
        "{\"camera\": {\"resolution\": {\"width\": 1920,\"height\": 1080, },\"position\": {\"x\": 0,\"y\": -100,\"z\": 20, },\"rotation\": {\"x\": 0,\"y\": 0,\"z\": 0, },\"fov\": 70, }, }";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &camera = root.at("camera");
    const std::unique_ptr<raytracer::Camera> camera_ptr = create_camera(camera);

    cr_assert_not_null(camera_ptr);
}

Test(primitive_factory, test_primitive_factory_camera)
{
    const std::string input =
        "{\"render\": {\"background-color\": { \"r\": 0, \"g\": 0, \"b\": 0 },\"antialiasing\": {\"type\": \"supersampling\",\"samples\": 4},\"output\": {\"file\": \"output.ppm\",\"format\": \"ppm\"},\"max-depth\": 10},\"camera\": {\"resolution\": { \"width\": 1920, \"height\": 1080 },\"position\": { \"x\": 0, \"y\": 0, \"z\": 10 },\"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 },\"fov\": 70},\"scene\": {\"materials\": [{\"name\": \"light\",\"reflectivity\": 0.0,\"transparency\": 0.0,\"refractive-index\": 0.0,\"emissive-intensity\": 10.0, \"shininess\": 50.0}],\"primitives\": {\"spheres\": [{\"position\": { \"x\": 4.8, \"y\": 4.17, \"z\": -8.12 },\"radius\": 2,\"material\": \"light\",\"color\": { \"r\": 0.90, \"g\": 0.0, \"b\": 0.0 }}]}}}";

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

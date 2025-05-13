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

Test(create_camera, test_camera)
{
    const std::string input =
        "{\"camera\": {\"resolution\": { \"width\": 1920, \"height\": 1080 },\"position\": { \"x\": 0, \"y\": 0, \"z\": -3 },\"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 },\"fov\": 72}}";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &camera = root.at("camera");
    const std::unique_ptr<raytracer::Camera> camera_ptr = create_camera(camera);

    cr_assert_not_null(camera_ptr);
}

Test(primitive_factory, test_primitive_factory)
{
    const std::string input =
        "{\"render\": {\"antialiasing\": {\"samples\": 5,\"mode\": \"classic\"},\"background-color\": { \"r\": 26, \"g\": 0, \"b\": 77 },\"lighting\": {\"gamma\": 3.0,\"ambient\": {\"coef\": 0.6,\"samples\": 10},\"diffuse\": 1.0,\"specular\": 0.6},\"max-depth\": 4,\"output\": {\"file\": \"output.ppm\",\"format\": \"ppm\"}},\"camera\": {\"resolution\": { \"width\": 1920, \"height\": 1080 },\"position\": { \"x\": 0, \"y\": 0, \"z\": -3 },\"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 },\"fov\": 72},\"scene\": {\"shapes\": {\"spheres\": [{\"position\": { \"x\": -1, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"specular\",\"color\": { \"r\": 0, \"g\": 255, \"b\": 0 }},{\"position\": { \"x\": 0, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"diffuse\",\"color\": { \"r\": 255, \"g\": 0, \"b\": 0 }},{\"position\": { \"x\": 1, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"dielectric\",\"refraction\": {\"inside\": 1.0,\"outside\": 1.52},\"color\": { \"r\": 0, \"g\": 0, \"b\": 255 }},{\"position\": { \"x\": 0, \"y\": -100.5, \"z\": -1 },\"radius\": 100,\"material\": \"dielectric\",\"refraction\": {\"inside\": 0.0,\"outside\": 0.0},\"color\": { \"r\": 255, \"g\": 255, \"b\": 255 }}]},\"lights\": {\"points\": [{\"position\": { \"x\": 0, \"y\": 2, \"z\": 1 },\"color\": { \"r\": 0, \"g\": 255, \"b\": 255 },\"intensity\": 5.0}]}}}";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &shapes = scene.at("shapes");
    const IShapesList &primitives_ptr = primitive_factory(shapes);

    cr_assert(primitives_ptr.begin() != primitives_ptr.end());
}

Test(light_factory, test_light_factory)
{
    const std::string input =
        "{\"render\": {\"antialiasing\": {\"samples\": 5,\"mode\": \"classic\"},\"background-color\": { \"r\": 26, \"g\": 0, \"b\": 77 },\"lighting\": {\"gamma\": 3.0,\"ambient\": {\"coef\": 0.6,\"samples\": 10},\"diffuse\": 1.0,\"specular\": 0.6},\"max-depth\": 4,\"output\": {\"file\": \"output.ppm\",\"format\": \"ppm\"}},\"camera\": {\"resolution\": { \"width\": 1920, \"height\": 1080 },\"position\": { \"x\": 0, \"y\": 0, \"z\": -3 },\"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 },\"fov\": 72},\"scene\": {\"shapes\": {\"spheres\": [{\"position\": { \"x\": -1, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"specular\",\"color\": { \"r\": 0, \"g\": 255, \"b\": 0 }},{\"position\": { \"x\": 0, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"diffuse\",\"color\": { \"r\": 255, \"g\": 0, \"b\": 0 }},{\"position\": { \"x\": 1, \"y\": 0, \"z\": -1 },\"radius\": 0.5,\"material\": \"dielectric\",\"refraction\": {\"inside\": 1.0,\"outside\": 1.52},\"color\": { \"r\": 0, \"g\": 0, \"b\": 255 }},{\"position\": { \"x\": 0, \"y\": -100.5, \"z\": -1 },\"radius\": 100,\"material\": \"dielectric\",\"refraction\": {\"inside\": 0.0,\"outside\": 0.0},\"color\": { \"r\": 255, \"g\": 255, \"b\": 255 }}]},\"lights\": {\"points\": [{\"position\": { \"x\": 0, \"y\": 2, \"z\": 1 },\"color\": { \"r\": 0, \"g\": 255, \"b\": 255 },\"intensity\": 5.0}]}}}";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &lights = scene.at("lights");
    const ILightsList &lights_ptr = light_factory(lights);

    cr_assert(lights_ptr.begin() != lights_ptr.end());
}

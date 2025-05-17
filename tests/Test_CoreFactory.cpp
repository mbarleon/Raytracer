/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_CoreFactory.cpp
*/

#include "../src/Core/CoreFactory.hpp"
#include "../src/Parser/Parser.hpp"
#include <criterion/criterion.h>
#include <fstream>
#include <string>
#include <sstream>

using JsonMap = std::unordered_map<std::string, raytracer::parser::JsonProto>;

std::string getConfigFileContent(const std::string &path)
{
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Unable to open file : " + path);
    }

    std::ostringstream oss;
    std::string ligne;
    while (std::getline(file, ligne)) {
        oss << ligne << '\n';
    }
    file.close();
    return oss.str();
}

Test(create_camera, test_camera)
{
    const std::string input =
        "{\"camera\": {\"resolution\": { \"width\": 1920, \"height\": 1080 },\"position\": { \"x\": 0, \"y\": 0, \"z\": -3 },\"rotation\": { \"x\": 0, \"y\": 0, \"z\": 0 },\"fov\": 72}}";

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &camera = root.at("camera");
    const auto camera_ptr = create_camera(camera);

    cr_assert_not_null(camera_ptr);
}

Test(primitive_factory, test_primitive_factory)
{
    const std::string input = getConfigFileContent("examples/minimal.jsonc");

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
    const std::string input = getConfigFileContent("examples/minimal.jsonc");

    auto it = input.begin();
    const auto end = input.end();
    const auto &jsonc = raytracer::parser::parseValue(it, end);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &lights = scene.at("lights");
    const ILightsList &lights_ptr = light_factory(lights);

    cr_assert(lights_ptr.begin() != lights_ptr.end());
}

/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Raytracer.cpp
*/

#include "../src/Core/Application.hpp"
#include "../src/Core/CoreFactory.hpp"
#include "../src/Elements/Scene/Lights/Directional/Directional.hpp"
#include "../src/Parser/Parser.hpp"
#include "../src/Parser/ParserTypes.hpp"
#include "Error.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/redirect.h>
#include <random>

static void redirect_stdout()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static raytracer::Camera create_camera()
{
    const math::Vector2u resolution(1920, 1080);
    const math::Point3D position(0.0, 0.0, 0.0);
    const math::Vector3D rotation(0.0, 0.0, 0.0);
    const raytracer::Camera camera(resolution, position, rotation, 90);

    return camera;
}

// clang-format off
namespace raytracer {

struct __scene_objects {
    IShapesList shapes;
    ILightsList lights;
    RenderConfig render_config;
    std::unique_ptr<Camera> camera;
};

static const __scene_objects create_objects()
{
    using ParsedJson = parser::JsonProto;
    using JsonMap = std::unordered_map<std::string, ParsedJson>;
    using Shapes = std::vector<ParsedJson>;
    using ILightsList = std::vector<std::shared_ptr<light::ILight>>;
    using IShapesList = std::vector<std::shared_ptr<shape::IShape>>;

    const parser::JsonValue jsonc = parser::parseJsonc("../tests/test-examples/test.jsonc");
    const JsonMap &root = std::get<JsonMap>(jsonc);
    const ParsedJson &render = root.at("render");
    const ParsedJson &camera = root.at("camera");
    const JsonMap &scene = std::get<JsonMap>(root.at("scene").value);
    const ParsedJson &shapes = scene.at("shapes");
    const ParsedJson &lights = scene.at("lights");

    IShapesList s = primitive_factory(shapes);
    ILightsList l = light_factory(lights);
    RenderConfig r = create_render(render);
    std::unique_ptr<Camera> c = create_camera(camera);

    return (const __scene_objects){
        .shapes = std::move(s),
        .lights = std::move(l),
        .render_config = std::move(r),
        .camera = std::move(c)
    };
}
}// namespace raytracer
// clang-format on

Test(Color, test_getBackgroundColor)
{
    const math::Vector3D vec3(23, 14, 24);
    const math::RGBColor background(1, 2, 3);
    const math::Vector3D dir = vec3.normalize();
    const double t = 0.5 * (dir._y + 1.0);
    const math::RGBColor expected = (1.0 - t) * math::RGBColor(0) + t * background;
    const math::RGBColor result = raytracer::getBackgroundColor(vec3, background);

    cr_assert_float_eq(result._x, expected._x, EPSILON);
    cr_assert_float_eq(result._y, expected._y, EPSILON);
    cr_assert_float_eq(result._z, expected._z, EPSILON);
}

Test(Color, test_getRayColor)
{
    const auto slrc = raytracer::create_objects();
    const math::Ray ray(math::Point3D(0, 0, 0), math::Vector3D(1, 1, 1));
    std::mt19937 mt;
    const auto ls = raytracer::getRayColor(ray, slrc.shapes, slrc.lights, slrc.render_config, 0, mt, true, math::RGBColor(1.0));

    cr_assert_eq(ls.isDelta, false);
}

Test(Camera, test_camera_resolution, .init = redirect_stdout)
{
    const raytracer::Camera camera = create_camera();

    cr_assert_eq(camera.getResolution()._x, 1920);
    cr_assert_eq(camera.getResolution()._y, 1080);
}

Test(Camera, generateRay, .init = redirect_stdout)
{
    const raytracer::Camera camera = create_camera();
    math::Ray ray;

    camera.generateRay(42.0, 69.0, ray);
    cr_assert_eq(ray._origin == math::Point3D(0.0, 0.0, 0.0), true);
}

Test(Application, test_application_filename, .init = redirect_stdout)
{
    try {
        raytracer::core::Application __attribute__((unused)) app(nullptr, false);
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "Could not open ");
    }
    raytracer::core::Application __attribute__((unused)) app2("../tests/test-examples/test.jsonc", false);
    raytracer::core::Application __attribute__((unused)) app3("../tests/test-examples/test.jsonc", true);
}

// clang-format off
Test(Lighting, test_ambient_occlusion)
{
    const auto slrc = raytracer::create_objects();
    const math::Point3D p3(0.0, 4.2, 1.3);
    const math::Vector3D v3(0.2, 3.1, 2.3);
    const math::Intersect isect = {
        .object = slrc.shapes.front().get(),
        .point = p3,
        .normal = v3.normalize(),
        .distance = 3.0
    };
    std::mt19937 rng;
    const double ao = raytracer::ambientOcclusion(isect, slrc.shapes, 4, rng, true);

    cr_assert_eq(ao, 0.875);
}

Test(Lighting, test_phong_direct)
{
    const auto slrc = raytracer::create_objects();
    const math::Point3D p3(0.0, 4.2, 1.3);
    const math::Vector3D v3(0.2, 3.1, 2.3);
    const math::Intersect isect = {
        .object = slrc.shapes.front().get(),
        .point = p3,
        .normal = v3.normalize(),
        .distance = 3.0
    };
    std::mt19937 rng;
    const math::RGBColor rgb = raytracer::phongDirect(isect, v3, slrc.lights, slrc.shapes, slrc.render_config, rng, false);

    cr_assert_eq(rgb._x, 0);
    cr_assert_eq(rgb._z, 0);
}

Test(Directional, test_directional_light__ctor__)
{
    const math::RGBColor rgb(23, 32, 12);
    const math::Vector3D dir(231, -232, 32);
    const double intensity = 49.3;
    const raytracer::light::Directional directional(rgb, dir, intensity);

    cr_assert_eq(directional.getIntensity(), 49.3);
    cr_assert_eq(directional.getColor() == rgb, true);
}

Test(Directional, test_directional_light__sample)
{
    const math::RGBColor rgb(23, 32, 12);
    const math::Vector3D dir(231, -232, 32);
    const double intensity = 49.3;
    const raytracer::light::Directional directional(rgb, dir, intensity);
    const math::Point3D samplePoint(32, 22, 213);

    const auto bsdf = directional.sample(samplePoint);

    const math::RGBColor expectedRadiance(1133.9, 1577.6, 591.6);
    const double expectedPdf = 1.0;
    const bool expectedIsDelta = true;
    const math::Vector3D expectedDirection(-0.702231, 0.705271, -0.0972788);

    constexpr double epsilon = 0.0001;

    cr_assert_float_eq(bsdf.radiance._x, expectedRadiance._x, epsilon);
    cr_assert_float_eq(bsdf.radiance._y, expectedRadiance._y, epsilon);
    cr_assert_float_eq(bsdf.radiance._z, expectedRadiance._z, epsilon);

    cr_assert_float_eq(bsdf.pdf, expectedPdf, epsilon);
    cr_assert_eq(bsdf.isDelta, expectedIsDelta);

    cr_assert_float_eq(bsdf.direction._x, expectedDirection._x, epsilon);
    cr_assert_float_eq(bsdf.direction._y, expectedDirection._y, epsilon);
    cr_assert_float_eq(bsdf.direction._z, expectedDirection._z, epsilon);
}
// clang-format on

Test(Application, application_run)
{
    raytracer::core::Application app("../tests/test-examples/test.jsonc", false);

    app.runNoGUI();
}

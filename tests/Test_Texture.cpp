/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_SkyboxCube.cpp
*/

#include "Error.hpp"

#include "../src/Elements/Scene/Textures/Color/ColorTexture.hpp"
#include "../src/Elements/Scene/Textures/Image/ImageTexture.hpp"
#include "../src/Elements/Scene/Textures/Procedural/Chessboard/Chessboard.hpp"
#include "../src/Elements/Scene/Textures/Procedural/PerlinNoise/PerlinNoise.hpp"
#include "../src/Elements/Scene/Textures/Skybox/Cube/SkyboxCube.hpp"
#include "../src/Elements/Scene/Textures/Skybox/Panoramic/SkyboxPanoramic.hpp"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

static void redirect_all_stdout()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(SkyboxCube, test_skyboxcube__ctor__, .init = redirect_all_stdout)
{
    const raytracer::texture::SkyboxCube __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    try {
        const raytracer::texture::SkyboxCube __attribute__((unused)) skybox("zebijexistepashaha(:");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "Cannot load zebijexistepashaha(:");
    }
}

Test(SkyboxCube, test_skyboxcube__value__)
{
    const raytracer::texture::SkyboxCube __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    const math::Point3D p(0.0, 0.0, 0.0);
    const double u = 0.5;
    const double v = 0.5;
    const math::RGBColor color = skybox.value(p, u, v);

    cr_assert_float_eq(color._x, 0.168627, 0.0001);
    cr_assert_float_eq(color._y, 0.160784, 0.0001);
    cr_assert_float_eq(color._z, 0.105882, 0.0001);
}

Test(SkyboxCube, test_skyboxcube__value__2)
{
    const raytracer::texture::SkyboxCube __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    const math::Point3D p(1.0, -21.0, 32.0);
    const double u = 0.5;
    const double v = -0.5;
    const math::RGBColor color = skybox.value(p, u, v);

    cr_assert_float_eq(color._x, 0.52549, 0.0001);
    cr_assert_float_eq(color._y, 0.439216, 0.0001);
    cr_assert_float_eq(color._z, 0.219608, 0.0001);
}

Test(SkyboxPanoramic, test_skyboxpanoramic__ctor__, .init = redirect_all_stdout)
{
    const raytracer::texture::SkyboxPanoramic __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    try {
        const raytracer::texture::SkyboxPanoramic __attribute__((unused)) skybox("zebijexistepashaha(:");
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "Cannot load zebijexistepashaha(:");
    }
}

Test(SkyboxPanoramic, test_skyboxpanoramic__value__)
{
    const raytracer::texture::SkyboxPanoramic __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    const math::Point3D p(0.0, 0.0, 0.0);
    const double u = 0.5;
    const double v = 0.5;
    const math::RGBColor color = skybox.value(p, u, v);

    cr_assert_float_eq(color._x, 0.392157, 0.0001);
    cr_assert_float_eq(color._y, 0.384314, 0.0001);
    cr_assert_float_eq(color._z, 0.368627, 0.0001);
}

Test(SkyboxPanoramic, test_skyboxpanoramic__value__2)
{
    const raytracer::texture::SkyboxPanoramic __attribute__((unused)) skybox("examples/skyboxes/panoramic/minecraft.png");
    const math::Point3D p(1.0, -21.0, 32.0);
    const double u = 0.5;
    const double v = -0.5;
    const math::RGBColor color = skybox.value(p, u, v);

    cr_assert_float_eq(color._x, 0.364706, 0.0001);
    cr_assert_float_eq(color._y, 0.305882, 0.0001);
    cr_assert_float_eq(color._z, 0.172549, 0.0001);
}

Test(ImageTexture, test_imagetexture__ctor__, .init = redirect_all_stdout)
{
    const raytracer::texture::ImageTexture __attribute__((unused)) image("examples/skyboxes/panoramic/minecraft.png", 1.0);
    try {
        const raytracer::texture::ImageTexture __attribute__((unused)) image("zebijexistepashaha(:", 1.0);
    } catch (const raytracer::exception::Error &e) {
        cr_assert_str_eq(e.what(), "Cannot load zebijexistepashaha(:");
    }
}

Test(ImageTexture, test_imagetexture__value__)
{
    const raytracer::texture::ImageTexture __attribute__((unused)) image("examples/skyboxes/panoramic/minecraft.png", 1.0);
    const math::Point3D p(0.0, 0.0, 0.0);
    const double u = 0.5;
    const double v = 0.5;
    const math::RGBColor color = image.value(p, u, v);

    cr_assert_float_eq(color._x, 0.392157, 0.0001);
    cr_assert_float_eq(color._y, 0.384314, 0.0001);
    cr_assert_float_eq(color._z, 0.368627, 0.0001);
}

Test(ColorTexture, test_colortexture__ctor__, .init = redirect_all_stdout)
{
    const raytracer::texture::ColorTexture __attribute__((unused)) color(math::RGBColor(0.5, 0.5, 0.5));
}

Test(ColorTexture, test_colortexture__value__)
{
    const raytracer::texture::ColorTexture __attribute__((unused)) color(math::RGBColor(0.5, 0.5, 0.5));

    cr_assert_eq(color.value(math::Point3D(0.0, 0.0, 0.0), 0.5, 0.5)._x, 0.5);
}

Test(Chessboard, test_chessboard__ctor__, .init = redirect_all_stdout)
{
    const raytracer::texture::Chessboard
        __attribute__((unused)) chessboard(math::RGBColor(0.5, 0.5, 0.5), math::RGBColor(0.0, 0.0, 0.0), 1.0);
}

Test(Chessboard, test_chessboard__value__)
{
    const raytracer::texture::Chessboard
        __attribute__((unused)) chessboard(math::RGBColor(0.5, 0.5, 0.5), math::RGBColor(4.0, 1.0, 4.0), 1.0);
    const math::Point3D p(4.0, 14.0, 64.0);
    const double u = 0.5;
    const double v = 0.31;
    const math::RGBColor color = chessboard.value(p, u, v);
    const math::RGBColor result(0.5, 0.5, 0.5);

    cr_assert_eq(color == result, true);
}

Test(PerlinNoise, test_perlinnoise__ctor__)
{
    const math::RGBColor color1(0.5, 0.5, 0.5);
    const math::RGBColor color2(0.0, 0.0, 0.0);

    const raytracer::texture::PerlinNoise __attribute__((unused)) perlin(color1, color2, 1.0);
}

Test(PerlinNoise, test_perlinnoise__value__)
{
    const math::RGBColor color1(0.5, 0.5, 0.5);
    const math::RGBColor color2(0.0, 0.0, 0.0);
    const raytracer::texture::PerlinNoise __attribute__((unused)) perlin(color1, color2, 1.0);
    const math::Point3D p(4.0, 14.0, 64.0);
    const double u = 0.5;
    const double v = 0.31;
    const math::RGBColor color = perlin.value(p, u, v);
    const math::RGBColor result(0.25, 0.25, 0.25);

    cr_assert_eq(color == result, true);
}

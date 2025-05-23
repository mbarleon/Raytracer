/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.cpp
*/

#include "CoreFactory.hpp"
#include "../Elements/Scene/Lights/Directional/Directional.hpp"
#include "../Elements/Scene/Lights/Point/Point.hpp"
#include "../Elements/Scene/Materials/BSDF/Dielectric/Dielectric.hpp"
#include "../Elements/Scene/Materials/BSDF/Diffuse/Diffuse.hpp"
#include "../Elements/Scene/Materials/BSDF/Metal/Metal.hpp"
#include "../Elements/Scene/Materials/BSDF/Specular/Specular.hpp"
#include "../Elements/Scene/Shapes/Cone/Cone.hpp"
#include "../Elements/Scene/Shapes/Plane/Plane.hpp"
#include "../Elements/Scene/Shapes/Rectangle/Rectangle.hpp"
#include "../Elements/Scene/Shapes/STL/STLShape.hpp"
#include "../Elements/Scene/Shapes/ScrewForm/ScrewForm.hpp"
#include "../Elements/Scene/Shapes/Sphere/Sphere.hpp"
#include "../Elements/Scene/Shapes/Cylinder/Cylinder.hpp"
#include "../Elements/Scene/Shapes/TangleCube/TangleCube.hpp"
#include "../Elements/Scene/Shapes/Torus/Torus.hpp"
#include "../Elements/Scene/Shapes/Triangle/Triangle.hpp"
#include "../Elements/Scene/Textures/Color/ColorTexture.hpp"
#include "../Elements/Scene/Textures/Image/ImageTexture.hpp"
#include "../Elements/Scene/Textures/Procedural/Chessboard/Chessboard.hpp"
#include "../Elements/Scene/Textures/Procedural/PerlinNoise/PerlinNoise.hpp"
#include "../Elements/Scene/Textures/Skybox/Panoramic/SkyboxPanoramic.hpp"
#include "Error.hpp"
#include "Macro.hpp"

#include <memory>

/**
 * @brief template function to extract a value of type <T> from ParsedJson
 * @details handles type checking and conversion for <T>
 *   with special handling for double (accepts int)
 * @param proto ParsedJson object
 * @return value of type <T>
 */
template<typename T>
T get_value(const ParsedJson &proto)
{
    if (std::holds_alternative<T>(proto.value)) {
        return std::get<T>(proto.value);
    }
    if constexpr (std::is_same_v<T, double>) {
        if (std::holds_alternative<int>(proto.value)) {
            return static_cast<double>(std::get<int>(proto.value));
        }
    }
    throw raytracer::exception::Error("Core",
        std::is_same_v<T, double> ? "Expected number (int or double)" : "Expected type not found");
}

/**
 * @brief template function to emplace shapes into IShapesList
 * @details handles the common logic of iterating over a JSON array of shapes
 * @param primitives JSON map containing the shapes
 * @param key the key in the JSON map (e.g., "spheres", "rectangles")
 * @param shapes the IShapesList to populate
 * @param creator the function to create the specific shape
 */
template<typename ShapeCreator>
unit_static void emplace_shapes(const JsonMap &primitives, const std::string &key, IShapesList &shapes, ShapeCreator creator)
{
    if (const auto it = primitives.find(key); it != primitives.end() && std::holds_alternative<Shapes>(it->second.value)) {

        const auto &shape_array = std::get<Shapes>(it->second.value);

        shapes.reserve(shapes.size() + shape_array.size());

        for (const auto &e : shape_array) {
            shapes.emplace_back(creator(e));
        }
    }
}

template<typename LightCreator>
unit_static void emplace_lights(const JsonMap &lights, const std::string &key, ILightsList &lightSrc, LightCreator creator)
{
    if (const auto it = lights.find(key); it != lights.end() && std::holds_alternative<Shapes>(it->second.value)) {

        const auto &light_array = std::get<Shapes>(it->second.value);

        lightSrc.reserve(lightSrc.size() + light_array.size());

        for (const auto &e : light_array) {
            lightSrc.emplace_back(creator(e));
        }
    }
}

/**
 * @brief get a string from ParsedJson
 * @details uses get_value template
 * @param proto ParsedJson object
 * @return string value
 */
unit_static std::string get_string(const ParsedJson &proto)
{
    return get_value<std::string>(proto);
}

/**
 * @brief clamp_color
 * @details clamp color component to [0, 255] and normalize to [0, 1]
 * @param component color component
 * @return Normalized color component
 */
unit_static double clamp_color(double component)
{
    if (component > MAX_COLOR) {
        component = MAX_COLOR;
    }
    if (component < MIN_COLOR) {
        component = MIN_COLOR;
    }
    return component / MAX_COLOR;
}

/**
 * @brief get RGB color from ParsedJson
 * @details uses get_value for JsonMap and double extraction
 * @param proto ParsedJson object
 * @return RGBColor object
 */
unit_static math::RGBColor get_color(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);

    return math::RGBColor(clamp_color(get_value<double>(obj.at("r"))), clamp_color(get_value<double>(obj.at("g"))),
        clamp_color(get_value<double>(obj.at("b"))));
}

/**
 * @brief get 3D vector from ParsedJson
 * @details use get_value for JsonMap and double extraction
 * @param proto ParsedJson object
 * @return Vector3D object
 */
unit_static math::Vector3D get_vec3D(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);

    return math::Vector3D(get_value<double>(obj.at("x")), get_value<double>(obj.at("y")), get_value<double>(obj.at("z")));
}

/**
 * @brief get 2D unsigned vector from ParsedJson
 * @details uses get_value for JsonMap and double extraction
 * @param proto ParsedJson object
 * @return Vector2u object
 */
unit_static math::Vector2u get_vec2u(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const double x = get_value<double>(obj.at("width"));
    const double y = get_value<double>(obj.at("height"));

    return math::Vector2u{static_cast<uint>(x), static_cast<uint>(y)};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief get material from ParsedJson
 * @details uses get_string for material name lookup
 * @param proto ParsedJson object
 * @return struct Material
 */
unit_static raytracer::material::Material get_material(const JsonMap &obj)
{
    const auto &mat_obj = get_value<JsonMap>(obj.at("material"));
    const std::string materialId = get_string(mat_obj.at("type"));
    std::shared_ptr<raytracer::material::BSDF> bsdf;

    try {
        if (materialId == std::string("diffuse")) {
            bsdf = std::make_shared<raytracer::material::DiffuseBSDF>();
        } else if (materialId == std::string("specular")) {
            bsdf = std::make_shared<raytracer::material::SpecularBSDF>();
        } else if (materialId == std::string("dielectric")) {
            const auto &refract_obj = get_value<JsonMap>(mat_obj.at("refraction"));
            const double ior_in = get_value<double>(refract_obj.at("inside"));
            const double ior_out = get_value<double>(refract_obj.at("outside"));
            bsdf = std::make_shared<raytracer::material::DielectricBSDF>(ior_out, ior_in);
        } else if (materialId == std::string("metal")) {
            const double roughness = get_value<double>(mat_obj.at("roughness"));
            bsdf = std::make_shared<raytracer::material::MetalBSDF>(roughness);
        } else {
            throw raytracer::exception::Error("Core", "Unknown material '", materialId, "'");
        }
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Bad material allocation", e.what());
    }
    return raytracer::material::Material(bsdf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

unit_static std::shared_ptr<raytracer::texture::ITexture> get_normal_map(const JsonMap &obj, double &strength)
{
    if (obj.find("normal") == obj.end()) {
        strength = 1.0;
        return nullptr;
    }

    try {
        const auto &normal_obj = get_value<JsonMap>(obj.at("normal"));
        const std::string filename = get_string(normal_obj.at("filename"));
        const double scale = get_value<double>(normal_obj.at("scale"));

        strength = get_value<double>(normal_obj.at("strength"));
        return std::make_shared<raytracer::texture::ImageTexture>(filename, scale);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Bad normal map allocation", e.what());
    }
    return nullptr;
}

unit_static std::shared_ptr<raytracer::texture::ITexture> get_texture(const JsonMap &obj)
{
    const auto &texture_obj = get_value<JsonMap>(obj.at("texture"));
    const std::string textureId = get_string(texture_obj.at("type"));

    try {
        if (textureId == std::string("color")) {
            const auto &color_obj = get_value<JsonMap>(texture_obj.at("colors"));
            const math::RGBColor color1 = get_color(color_obj.at("1"));
            return std::make_shared<raytracer::texture::ColorTexture>(color1);
        } else if (textureId == std::string("chessboard")) {
            const double scale = get_value<double>(texture_obj.at("scale"));
            const auto &color_obj = get_value<JsonMap>(texture_obj.at("colors"));
            const math::RGBColor color1 = get_color(color_obj.at("1"));
            const math::RGBColor color2 = get_color(color_obj.at("2"));
            return std::make_shared<raytracer::texture::Chessboard>(color1, color2, scale);
        } else if (textureId == std::string("perlin-noise")) {
            const double scale = get_value<double>(texture_obj.at("scale"));
            const auto &color_obj = get_value<JsonMap>(texture_obj.at("colors"));
            const math::RGBColor color1 = get_color(color_obj.at("1"));
            const math::RGBColor color2 = get_color(color_obj.at("2"));
            return std::make_shared<raytracer::texture::PerlinNoise>(color1, color2, scale);
        } else if (textureId == std::string("image")) {
            const double scale = get_value<double>(texture_obj.at("scale"));
            const std::string file = get_string(texture_obj.at("filename"));
            return std::make_shared<raytracer::texture::ImageTexture>(file, scale);
        } else {
            throw raytracer::exception::Error("Core", "Unknown texture '", textureId, "'");
        }
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Bad texture allocation", e.what());
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

unit_static std::shared_ptr<raytracer::light::ILight> create_light_point(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Point3D position = get_vec3D(obj.at("position"));
    const math::RGBColor color = get_color(obj.at("color"));
    const double intensity = get_value<double>(obj.at("intensity"));

    return std::make_shared<raytracer::light::Point>(color, position, intensity);
}

unit_static std::shared_ptr<raytracer::light::ILight> create_light_directional(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D direction = get_vec3D(obj.at("direction"));
    const math::RGBColor color = get_color(obj.at("color"));
    const double intensity = get_value<double>(obj.at("intensity"));

    return std::make_shared<raytracer::light::Directional>(color, direction, intensity);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief create a shape from ParsedJson
* @details uses get_value for JsonMap and other extractions
* @param shape out shared pointer to shape
* @param obj const ParsedJson object
* @return void
*/
unit_static void create_shape(const std::shared_ptr<raytracer::shape::IShape> &shape, const JsonMap &obj)
{
    double strength;
    const double shininess = get_value<double>(obj.at("shininess"));
    const auto normal = get_normal_map(obj, strength);
    const math::Vector3D rotation = get_vec3D(obj.at("rotation"));

    shape->setRotation(rotation);
    shape->setMaterial(get_material(obj));
    shape->setTexture(get_texture(obj));
    shape->setNormalMap(normal, strength);
    shape->setShininess(shininess);
}

/**
 * @brief create a torus from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to torus
 */
unit_static std::shared_ptr<raytracer::shape::Torus> create_torus(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D position = get_vec3D(obj.at("origin"));
    const double minorRadius = get_value<double>(obj.at("minor-radius"));
    const double majorRadius = get_value<double>(obj.at("major-radius"));
    std::shared_ptr<raytracer::shape::Torus> torus = nullptr;

    try {
        torus = std::make_shared<raytracer::shape::Torus>(position, majorRadius, minorRadius);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Torus bad allocation", e.what());
    }
    create_shape(torus, obj);
    return torus;
}

/**
* @brief create a cone from ParsedJson
* @details uses get_value for JsonMap and other extractions
* @param proto ParsedJson object
* @return shared pointer to cone
*/
unit_static std::shared_ptr<raytracer::shape::Cone> create_cone(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D apex = get_vec3D(obj.at("apex"));
    const math::Vector3D direction = get_vec3D(obj.at("direction"));
    const double angle = get_value<double>(obj.at("angle")) * DEGREE_TO_RADIANT;
    const double height = get_value<double>(obj.at("height"));
    std::shared_ptr<raytracer::shape::Cone> cone = nullptr;

    try {
        cone = std::make_shared<raytracer::shape::Cone>(apex, direction, angle, height);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Cone bad allocation", e.what());
    }
    create_shape(cone, obj);
    return cone;
}

/**
 * @brief create a tangle cube from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to tangle cube
 */
unit_static std::shared_ptr<raytracer::shape::TangleCube> create_tangle_cube(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D position = get_vec3D(obj.at("origin"));
    const double size = get_value<double>(obj.at("size"));
    const double thickness = get_value<double>(obj.at("thickness"));
    std::shared_ptr<raytracer::shape::TangleCube> cube = nullptr;

    try {
        cube = std::make_shared<raytracer::shape::TangleCube>(position, size, thickness);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Tangle cube bad allocation", e.what());
    }
    create_shape(cube, obj);
    return cube;
}

/**
 * @brief create a screw form from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to screw form
 */
unit_static std::shared_ptr<raytracer::shape::ScrewForm> create_screw_form(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D position = get_vec3D(obj.at("origin"));
    const double width = get_value<double>(obj.at("width"));
    const double freq = get_value<double>(obj.at("freq"));
    const unsigned maxSteps = static_cast<unsigned>(get_value<int>(obj.at("max-steps")));
    const double maxDistance = get_value<double>(obj.at("max-distance"));
    std::shared_ptr<raytracer::shape::ScrewForm> cube = nullptr;

    try {
        cube = std::make_shared<raytracer::shape::ScrewForm>(position, width, freq, maxSteps, maxDistance);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Screw form bad allocation", e.what());
    }
    create_shape(cube, obj);
    return cube;
}

/**
 * @brief create a sphere from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to Sphere
 */
unit_static std::shared_ptr<raytracer::shape::Sphere> create_sphere(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D position = get_vec3D(obj.at("position"));
    const double radius = get_value<double>(obj.at("radius"));
    std::shared_ptr<raytracer::shape::Sphere> sphere = nullptr;

    try {
        sphere = std::make_shared<raytracer::shape::Sphere>(position, radius);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Sphere bad allocation", e.what());
    }
    create_shape(sphere, obj);
    return sphere;
}

/**
 * @brief creeate a rectangle from ParsedJso
 * @details Uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to Rectangle
 */
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D origin = get_vec3D(obj.at("origin"));
    const double height = get_value<double>(obj.at("height"));
    const double width = get_value<double>(obj.at("width"));
    const double depth = get_value<double>(obj.at("depth"));
    std::shared_ptr<raytracer::shape::Rectangle> rectangle = nullptr;

    try {
        rectangle = std::make_shared<raytracer::shape::Rectangle>(origin, height, width, depth);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Rectangle bad allocation", e.what());
    }
    create_shape(rectangle, obj);
    return rectangle;
}

/**
* @brief create a triangle from ParsedJson
* @details uses get_value for JsonMap and other extractions
* @param proto ParsedJson object
* @return shared pointer to Triangle
*/
unit_static std::shared_ptr<raytracer::shape::Triangle> create_triangle(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Point3D p0 = get_vec3D(obj.at("p0"));
    const math::Point3D p1 = get_vec3D(obj.at("p1"));
    const math::Point3D p2 = get_vec3D(obj.at("p2"));
    std::shared_ptr<raytracer::shape::Triangle> tri = nullptr;

    try {
        tri = std::make_shared<raytracer::shape::Triangle>(p0, p1, p2);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Triangle bad allocation", e.what());
    }
    create_shape(tri, obj);
    return tri;
}

/**
 * @brief create a plane from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to Plane
 */
unit_static std::shared_ptr<raytracer::shape::Plane> create_plane(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const double position = get_value<double>(obj.at("position"));
    const std::string str = get_string(obj.at("axis"));

    if (str[0] != 'X' && str[0] != 'Y' && str[0] != 'Z') {
        throw raytracer::exception::Error("Core", "Invalid plane axis");
    }

    std::shared_ptr<raytracer::shape::Plane> plane = nullptr;

    try {
        plane = std::make_shared<raytracer::shape::Plane>(str[0], position);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Plane bad allocation", e.what());
    }
    create_shape(plane, obj);
    return plane;
}

/**
 * @brief create a cylinder from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @return shared pointer to Cylinder
 */
unit_static std::shared_ptr<raytracer::shape::Cylinder> create_cylinder(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Point3D base_center = get_vec3D(obj.at("origin"));
    const math::Vector3D axis = get_vec3D(obj.at("axis"));
    const double radius = get_value<double>(obj.at("radius"));
    const double height = get_value<double>(obj.at("height"));
    const raytracer::material::Material material = get_material(obj);

    const std::shared_ptr<raytracer::shape::Cylinder> cylinder =
        std::make_shared<raytracer::shape::Cylinder>(base_center, axis, radius, height);

    cylinder.get()->setMaterial(material);
    return cylinder;
}

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::STLShape> create_stl(const ParsedJson &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const math::Vector3D origin = get_vec3D(obj.at("origin"));
    const math::Vector3D rotation = get_vec3D(obj.at("rotation"));
    const std::string filename = get_string(obj.at("filename"));
    const auto scale = static_cast<float>(get_value<double>(obj.at("scale")));
    std::shared_ptr<raytracer::shape::STLShape> stl = nullptr;

    try {
        stl = std::make_shared<raytracer::shape::STLShape>(origin, rotation, filename.c_str(), scale);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "STL bad allocation", e.what());
    }
    create_shape(stl, obj);
    return stl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief entry Point: Create camera
 * @details uses get_value for JsonMap and other extractions
 * @param camera_json ParsedJson object
 * @return unique pointer to Camera
 */
std::unique_ptr<raytracer::Camera> create_camera(const ParsedJson &camera_json)
{
    const auto &obj = get_value<JsonMap>(camera_json);
    const math::Vector2u vec2 = get_vec2u(obj.at("resolution"));
    const math::Point3D pos = get_vec3D(obj.at("position"));
    math::Vector3D rot = get_vec3D(obj.at("rotation"));

    rot._x = rot._x * DEGREE_TO_RADIANT;
    rot._y = rot._y * DEGREE_TO_RADIANT;
    rot._z = rot._z * DEGREE_TO_RADIANT;

    const uint fov = static_cast<uint>(get_value<double>(obj.at("fov")));

    return std::make_unique<raytracer::Camera>(vec2, pos, rot, fov);
}

/**
 * @brief entry Point: Create render settings
 * @details uses get_value for JsonMap and other extractions
 * @param render_json ParsedJson object
 * @return unique pointer to Render
 */
raytracer::RenderConfig create_render(const ParsedJson &render_json)
{
    const auto &obj = get_value<JsonMap>(render_json);
    const auto &anti_obj = get_value<JsonMap>(obj.at("antialiasing"));

    const raytracer::Antialiasing anti = {static_cast<unsigned int>(get_value<int>(anti_obj.at("samples"))),
        get_string(anti_obj.at("mode"))};

    const std::string background = get_string(obj.at("skybox"));

    const auto &light_obj = get_value<JsonMap>(obj.at("lighting"));
    const auto &light_ambient_obj = get_value<JsonMap>(light_obj.at("ambient"));
    const raytracer::AmbientOcclusion ambient = {get_value<double>(light_ambient_obj.at("coef")),
        static_cast<unsigned int>(get_value<int>(light_ambient_obj.at("samples")))};
    const raytracer::Lighting light = {get_value<double>(light_obj.at("gamma")), get_value<double>(light_obj.at("extra-shadow")),
        ambient, get_value<double>(light_obj.at("diffuse")), get_value<double>(light_obj.at("specular"))};

    const unsigned int mdepth = static_cast<uint>(get_value<int>(obj.at("max-depth")));

    const auto &out_obj = get_value<JsonMap>(obj.at("output"));
    const raytracer::RenderOutput output = {get_string(out_obj.at("file")), get_string(out_obj.at("format"))};

    return {anti, std::make_shared<raytracer::texture::SkyboxPanoramic>(background), light, mdepth, output};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

ILightsList light_factory(const ParsedJson &json_lights)
{
    const auto &lights = get_value<JsonMap>(json_lights);
    ILightsList lightSrc;

    try {
        emplace_lights(lights, "points", lightSrc, create_light_point);
        emplace_lights(lights, "directionals", lightSrc, create_light_directional);
    } catch (const std::bad_alloc &e) {
        throw raytracer::exception::Error("Core", "Light bad allocation", e.what());
    }
    return lightSrc;
}

/**
 * @brief primitive factory entry point
 * @details uses emplace_shapes template to handle spheres, rectangles, and planes
 * @param json_primitives ParsedJson object
 * @param materials MaterialsList to use for material creation
 * @return IShapesList
 */
IShapesList primitive_factory(const ParsedJson &json_primitives)
{
    const auto &primitives = get_value<JsonMap>(json_primitives);
    IShapesList shapes;

    emplace_shapes(primitives, "spheres", shapes, create_sphere);
    emplace_shapes(primitives, "rectangles", shapes, create_rectangle);
    emplace_shapes(primitives, "planes", shapes, create_plane);
    emplace_shapes(primitives, "cylinders", shapes, create_cylinder);
    emplace_shapes(primitives, "stl", shapes, create_stl);
    emplace_shapes(primitives, "toruses", shapes, create_torus);
    emplace_shapes(primitives, "screw-forms", shapes, create_screw_form);
    emplace_shapes(primitives, "tangle-cubes", shapes, create_tangle_cube);
    emplace_shapes(primitives, "triangles", shapes, create_triangle);
    emplace_shapes(primitives, "cones", shapes, create_cone);

    return shapes;
}

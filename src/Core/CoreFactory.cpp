/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.cpp
*/

#include "CoreFactory.hpp"
#include "../../include/Logger.hpp"
#include "../Elements/Scene/Shapes/Plane.hpp"
#include "../Elements/Scene/Shapes/Rectangle.hpp"
#include "../Elements/Scene/Shapes/Sphere.hpp"
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
 * @param materials the MaterialsList to use for material creation
 */
template<typename ShapeCreator>
unit_static void emplace_shapes(const JsonMap &primitives, const std::string &key, IShapesList &shapes, ShapeCreator creator,
    const MaterialsList &materials)
{
    const auto it = primitives.find(key);

    if (it != primitives.end() && std::holds_alternative<Shapes>(it->second.value)) {

        const auto &shape_array = std::get<Shapes>(it->second.value);

        shapes.reserve(shapes.size() + shape_array.size());

        for (const auto &e : shape_array) {
            shapes.emplace_back(creator(e, materials));
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
unit_static raytracer::RGBColor get_color(const ParsedJson &proto)
{
    const auto &obj = get_value<JsonMap>(proto);

    return raytracer::RGBColor(clamp_color(get_value<double>(obj.at("r"))), clamp_color(get_value<double>(obj.at("g"))),
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

/**
 * @brief get material from ParsedJson
 * @details uses get_string for material name lookup
 * @param proto ParsedJson object
 * @param materials MaterialsList to search
 * @return shared pointer to Material
 */
unit_static std::shared_ptr<raytracer::Material> get_material(const ParsedJson &proto, const MaterialsList &materials)
{
    const std::string mat_str = get_string(proto);
    const auto it = materials.find(mat_str);

    if (it == materials.end()) {
        throw raytracer::exception::Error("Core", "Undefined material");
    }
    return it->second;
}

/**
 * @brief create a material from ParsedJson
 * @details uses get_value for all extractions
 * @param proto ParsedJson object
 * @param materials MaterialsList to add the material to
 * @return none <|> adds material to MaterialsList
 */
unit_static void create_material(const ParsedJson &proto, MaterialsList &materials)
{
    const auto &obj = get_value<JsonMap>(proto);
    const std::string name = get_string(obj.at("name"));
    const double reflectivity = get_value<double>(obj.at("reflectivity"));
    const double transparency = get_value<double>(obj.at("transparency"));
    const double refr_index = get_value<double>(obj.at("refractive-index"));
    const double em_intensity = get_value<double>(obj.at("emissive-intensity"));
    const double shininess = get_value<double>(obj.at("shininess"));
    const std::shared_ptr<raytracer::Material> material =
        std::make_shared<raytracer::Material>(reflectivity, transparency, refr_index, em_intensity, shininess);

    materials[name] = material;
    raytracer::logger::debug("Material was built: { name: ", name, ", reflectivity: ", reflectivity,
        ", transparency: ", transparency, ", refractive-index: ", refr_index, ", emissive-intensity: ", em_intensity,
        ", shininess: ", shininess, " }.");
}

/**
 * @brief create a sphere from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @param materials MaterialsList to use for material creation
 * @return shared pointer to Sphere
 */
unit_static std::shared_ptr<raytracer::shape::Sphere> create_sphere(const ParsedJson &proto, const MaterialsList &materials)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D position = get_vec3D(obj.at("position"));
    const double radius = get_value<double>(obj.at("radius"));
    const std::shared_ptr<raytracer::Material> material = get_material(obj.at("material"), materials);
    const raytracer::RGBColor color = get_color(obj.at("color"));
    const std::shared_ptr<raytracer::shape::Sphere> sphere = std::make_shared<raytracer::shape::Sphere>(position, radius);

    sphere.get()->setMaterial(material);
    sphere.get()->setColor(color);
    return sphere;
}

/**
 * @brief creeate a rectangle from ParsedJso
 * @details Uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @param materials MaterialsList to use for material creation
 * @return shared pointer to Rectangle
 */
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const ParsedJson &proto, const MaterialsList &materials)
{
    const auto &obj = get_value<JsonMap>(proto);
    const math::Vector3D origin = get_vec3D(obj.at("origin"));
    const math::Vector3D bottom_side = get_vec3D(obj.at("bottom_side"));
    const math::Vector3D left_side = get_vec3D(obj.at("left_side"));
    const std::shared_ptr<raytracer::Material> material = get_material(obj.at("material"), materials);
    const raytracer::RGBColor color = get_color(obj.at("color"));
    const std::shared_ptr<raytracer::shape::Rectangle> rectangle =
        std::make_shared<raytracer::shape::Rectangle>(origin, bottom_side, left_side);

    rectangle.get()->setMaterial(material);
    rectangle.get()->setColor(color);
    return rectangle;
}

/**
 * @brief create a plane from ParsedJson
 * @details uses get_value for JsonMap and other extractions
 * @param proto ParsedJson object
 * @param materials MaterialsList to use for material creation
 * @return shared pointer to Plane
 */
unit_static std::shared_ptr<raytracer::shape::Plane> create_plane(const ParsedJson &proto, const MaterialsList &materials)
{
    const auto &obj = get_value<JsonMap>(proto);
    const double position = get_value<double>(obj.at("position"));
    const std::shared_ptr<raytracer::Material> material = get_material(obj.at("material"), materials);
    const raytracer::RGBColor color = get_color(obj.at("color"));
    const std::string str = get_string(obj.at("axis"));

    if (str[0] != 'X' && str[0] != 'Y' && str[0] != 'Z') {
        throw raytracer::exception::Error("Core", "Invalid plane axis");
    }

    const std::shared_ptr<raytracer::shape::Plane> plane = std::make_shared<raytracer::shape::Plane>(str[0], position);

    plane.get()->setMaterial(material);
    plane.get()->setColor(color);
    return plane;
}

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
std::unique_ptr<raytracer::Render> create_render(const ParsedJson &render_json)
{
    const auto &obj = get_value<JsonMap>(render_json);
    const auto &anti_obj = get_value<JsonMap>(obj.at("antialiasing"));
    const raytracer::Antialiasing anti = {get_string(anti_obj.at("type")),
        static_cast<unsigned int>(get_value<double>(anti_obj.at("samples")))};

    const raytracer::RGBColor background = get_color(obj.at("background-color"));

    const auto &ambi_obj = get_value<JsonMap>(obj.at("lighting"));
    const raytracer::Lighting ambi = {get_value<double>(ambi_obj.at("ambient")), get_value<double>(ambi_obj.at("diffuse")),
        get_value<double>(ambi_obj.at("specular"))};

    const double mdepth = static_cast<uint>(get_value<double>(obj.at("max-depth")));

    const auto &out_obj = get_value<JsonMap>(obj.at("output"));
    const raytracer::RenderOutput output = {get_string(out_obj.at("file")), get_string(out_obj.at("format"))};

    return std::make_unique<raytracer::Render>(anti, background, ambi, mdepth, output);
}

/**
 * @brief material factory entry point
 * @details unchanged, relies on create_material
 * @param json_scene ParsedJson object
 * @return MaterialsList
 */
MaterialsList material_factory(const ParsedJson &json_scene)
{
    const auto &scene = get_value<JsonMap>(json_scene);
    MaterialsList materials;
    const auto material_it = scene.find("materials");

    if (material_it != scene.end() && std::holds_alternative<Shapes>(material_it->second.value)) {

        const auto &mats = std::get<Shapes>(material_it->second.value);

        materials.reserve(mats.size());
        for (const auto &e : mats) {
            create_material(e, materials);
        }
    }

    return materials;
}

/**
 * @brief primitive factory entry point
 * @details uses emplace_shapes template to handle spheres, rectangles, and planes
 * @param json_primitives ParsedJson object
 * @param materials MaterialsList to use for material creation
 * @return IShapesList
 */
IShapesList primitive_factory(const ParsedJson &json_primitives, const MaterialsList &materials)
{
    const auto &primitives = get_value<JsonMap>(json_primitives);
    IShapesList shapes;

    emplace_shapes(primitives, "spheres", shapes, create_sphere, materials);
    emplace_shapes(primitives, "rectangles", shapes, create_rectangle, materials);
    emplace_shapes(primitives, "planes", shapes, create_plane, materials);

    return shapes;
}

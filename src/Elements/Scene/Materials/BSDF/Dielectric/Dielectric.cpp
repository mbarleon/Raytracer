/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Dielectric
*/

#include "Dielectric.hpp"
#include <random>

raytracer::material::DielectricBSDF::DielectricBSDF(double etaExt, double etaInt,
    const math::RGBColor &absorptionCoeff) : etaExt(etaExt), etaInt(etaInt),
    absorptionCoeff(absorptionCoeff)
{
}

math::RGBColor raytracer::material::DielectricBSDF::sample(const math::Vector3D &wo,
    const math::Vector3D &normal, math::Vector3D &wi, double &pdf) const
{
    const bool entering = wo.dot(normal) < 0.0;
    const math::Vector3D n = entering ? normal : -normal;
    const double etaI = entering ? etaExt : etaInt;
    const double etaT = entering ? etaInt : etaExt;
    const double eta = etaI / etaT;

    const double cosThetaI = -wo.dot(n);
    const double sin2ThetaT = eta * eta * (1.0 - cosThetaI * cosThetaI);

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    if (sin2ThetaT > 1.0) {
        wi = reflect(wo, n);
        pdf = 1.0;
        return math::RGBColor(1.0);
    }

    const double cosThetaT = std::sqrt(1.0 - sin2ThetaT);
    math::Vector3D refracted = eta * (-wo) + (eta * cosThetaI - cosThetaT) * n;
    refracted = refracted.normalize();

    const double R0 = std::pow((etaI - etaT) / (etaI + etaT), 2.0);
    const double fresnel = R0 + (1.0 - R0) * std::pow(1.0 - std::fabs(cosThetaI), 5.0);

    if (dist(gen) < fresnel) {
        wi = reflect(wo, n);
        pdf = fresnel;
        return math::RGBColor(1.0);
    } else {
        wi = refracted;
        pdf = 1.0 - fresnel;

        const double distance = 1.0;
        math::RGBColor attenuation = math::RGBColor(
            std::exp(-absorptionCoeff._x * distance),
            std::exp(-absorptionCoeff._y * distance),
            std::exp(-absorptionCoeff._z * distance)
        );
        return attenuation;
    }
}

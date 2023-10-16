#pragma once

#include <memory>
#include "Color.h"
#include "Vec3.h"
#include "Texture.h"
namespace {
    std::unique_ptr<Texture> t = std::make_unique<Texture>("../assets/earth.jpg");
    std::unique_ptr<Texture> t_processed = std::make_unique<Texture>("");
}

class Sphere
{


public:
    Sphere() {

    };

    Sphere(const Vec3& center, float radius, const Color& color, const Color& emission = BLACK, bool specular = false, bool textured = false, const bool processed = false)
        : center(center)
        , radius(radius)
        , color(color)
        , emission(emission)
        , specular(specular)
        , textured(textured)
        , processed(processed)
    {
    }
    const Vec3 center;
    const float radius = 1.0f;
    const Color color = WHITE;
    const Color emission = BLACK;
    const bool specular = false;
    const bool textured = false;
    const bool processed = false;

    float intersect(const Vec3& origin, const Vec3& to) const;

    bool hasEmission() const {
        return !emission.equals(BLACK);
    }

    Color getEmmission(const Vec3& n) const {
        if(hasEmission()) {
            if(processed) {
                return t_processed->getPixelOnSphere(n, *t_processed);
            }
            return emission;
        }
        return BLACK;

    }

    Color getColor(const Vec3& n) const {
        if (textured) {
            return t->getPixelOnSphere(n, *t);
        }
        if(processed) {
            return t_processed->getPixelOnSphere(n, *t_processed);
        }
        return color;

    }

};
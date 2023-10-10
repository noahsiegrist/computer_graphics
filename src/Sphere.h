#pragma once
#include "Color.h"
#include "Vec3.h"

class Sphere
{
public:
    Sphere(const Vec3& center, float radius, const Color& color, const Color& emission = BLACK, bool specular = false);
    const Vec3 center;
    const float radius;
    const Color color = WHITE;
    const Color emission = BLACK;
    const bool specular = false;

    float intersect(const Vec3& origin, const Vec3& to) const;

    bool hasEmission() const {
        return !emission.equals(BLACK);
    }
};
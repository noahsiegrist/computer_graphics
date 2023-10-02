#pragma once
#include "Color.h"
#include "Vec3.h"

class Sphere
{
public:
    Sphere(const Vec3& center, double radius, const Color& color, const Color& emission = BLACK);
    const Vec3 center;
    const double radius;
    const Color color = WHITE;
    const Color emission = BLACK;

    Vec3 intersect(const Vec3& origin, const Vec3& to) const;

};
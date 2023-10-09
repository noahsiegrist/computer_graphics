#include "Sphere.h"
#include "Vec3.h"


Sphere::Sphere(const Vec3 &center, float radius, const Color &color, const Color &emission)
: center(center)
, radius(radius)
, color(color)
, emission(emission)
{
}

float Sphere::intersect(const Vec3 &origin, const Vec3 &to) const {
    const Vec3& co = origin - center;
    const Vec3& ray = to.normalize();

    float a = 1.f; // ray.dot(ray);  // This will be 1 if ray is normalized, but let's keep it for clarity.
    float b = 2.0f * ray.dot(co);
    float c = co.dot(co) - radius * radius;

    float first = b * b;
    float second = 4.0f * a * c;
    if(first >= second) {
        float discriminant = first - second;
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);
        if(t1 >= 0) {
            return t1;
        }
        if(t2 >= 0) {
            return t2;
        }
    }

    return -1;
}

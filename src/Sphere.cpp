#include "Sphere.h"
#include "Vec3.h"


Sphere::Sphere(const Vec3 &center, double radius, const Color &color, const Color &emission)
: center(center)
, radius(radius)
, color(color)
, emission(emission)
{
}

Vec3 Sphere::intersect(const Vec3 &origin, const Vec3 &to) const {
    const Vec3& co = origin - center;
    const Vec3& ray = to.normalize();

    double a = 1.f; // ray.dot(ray);  // This will be 1 if ray is normalized, but let's keep it for clarity.
    double b = 2.0f * ray.dot(co);
    double c = co.dot(co) - radius * radius;

    double first = b * b;
    double second = 4.0f * a * c;
    if(first >= second) {
        double discriminant = first - second;
        double sqrtDiscriminant = std::sqrt(discriminant);
        double t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        double t2 = (-b + sqrtDiscriminant) / (2.0f * a);
        if(t1 >= 0) {
            return ray * t1;
        }
        if(t2 >= 0) {
            return ray * t2;
        }
    }

    return {};
}

#pragma once

#include <vector>
#include <cmath>
#include <random>

class Vec3 {
public:
    const bool null = false;
    Vec3(float x, float y, float z) : vec({x, y, z}) {}
    Vec3() : null(true) {}
    const std::vector<float> vec;

    Vec3 operator-(const Vec3& other) const {
        return {vec[0] - other.vec[0], vec[1] - other.vec[1], vec[2] - other.vec[2]};
    }

    Vec3 operator-(float a) const {
        return {vec[0] - a, vec[1] - a, vec[2] - a};
    }

    Vec3 operator+(const Vec3& other) const {
        return {vec[0] + other.vec[0], vec[1] + other.vec[1], vec[2] + other.vec[2]};
    }

    Vec3 operator*(float factor) const {
        return {vec[0] * factor, vec[1] * factor, vec[2] * factor};
    }

    float dot(const Vec3& other) const {
        return vec[0] * other.vec[0] + vec[1] * other.vec[1] + vec[2] * other.vec[2];
    }

    float length() const {
        return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    }

    Vec3 normalize() const {
        float l = length();
        return {vec[0] / l, vec[1] / l, vec[2] / l};
    }

    Vec3 cross(const Vec3& other) const {
        return {vec[1] * other.vec[2] - vec[2] * other.vec[1],
                vec[2] * other.vec[0] - vec[0] * other.vec[2],
                vec[0] * other.vec[1] - vec[1] * other.vec[0]};
    }

    static Vec3 random();
};


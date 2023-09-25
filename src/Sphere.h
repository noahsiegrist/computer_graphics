#pragma once

class Sphere
{
public:
    Sphere(std::vector<float> center, float radius, std::vector<float> color);
    std::vector<float> center;
    float radius;
    std::vector<float> color;

};
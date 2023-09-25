#pragma once

#include <vector>

class Color
{

    std::vector<float> color;
public:
    Color (float r, float g, float b);
};

Color green = Color(0, 1, 0);
Color red = Color(1, 0, 0);
Color blue = Color(0, 0, 1);
Color white = Color(1, 1, 1);
Color black = Color(0, 0, 0);
Color yellow = Color(1, 1, 0);
Color gray = Color(0.5, 0.5, 0.5);
Color lightCyan = Color(0.5, 1, 1);

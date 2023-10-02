#pragma once

#include <vector>

class Color
{
private:
    std::vector<double> color;
public:
    Color (double r, double g, double b);
    Color multiply(Color color);
    Color add(Color color);
    double r() {return color[0];}
    double g() {return color[1];}
    double b() {return color[2];}
};

static Color GREEN = Color(0, 1, 0);
static Color RED = Color(1, 0, 0);
static Color BLUE = Color(0, 0, 1);
static Color WHITE = Color(1, 1, 1);
static Color BLACK = Color(0, 0, 0);
static Color YELLOW = Color(1, 1, 0);
static Color GREY = Color(0.5, 0.5, 0.5);
static Color LIGHTCYAN = Color(0.5, 1, 1);
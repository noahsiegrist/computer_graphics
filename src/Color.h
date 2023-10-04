#pragma once

#include <vector>

class Color
{
private:
    std::vector<double> color;
public:
    Color (double r, double g, double b);
    Color multiply(Color color) const;
    Color multiply(const double factor) const;
    Color add(Color color) const;
    Color clip() const;
    double r() {return color[0];}
    double g() {return color[1];}
    double b() {return color[2];}

    bool equals(Color color) const {
        return this->color[0] == color.color[0] && this->color[1] == color.color[1] && this->color[2] == color.color[2];
    }
};

static Color GREEN = Color(0, 1, 0);
static Color RED = Color(1, 0, 0);
static Color BLUE = Color(0, 0, 1);
static Color WHITE = Color(1, 1, 1);
static Color BLACK = Color(0, 0, 0);
static Color YELLOW = Color(1, 1, 0);
static Color GREY = Color(0.5, 0.5, 0.5);
static Color LIGHTCYAN = Color(0.5, 1, 1);
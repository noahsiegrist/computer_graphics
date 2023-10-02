#include "Color.h"

Color::Color(double r, double g, double b) {
    color= {r, g, b};
}

Color Color::add(Color color) {
    return {this->color[0] + color.color[0], this->color[1] + color.color[1], this->color[2] + color.color[2]};
}

Color Color::multiply(Color color) {
    return {this->color[0] * color.color[0], this->color[1] * color.color[1], this->color[2] * color.color[2]};
}
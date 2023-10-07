#include "Color.h"

Color::Color(double r, double g, double b) {
    color= {r, g, b};
}

Color Color::add(const Color& color) const {
    return {this->color[0] + color.color[0], this->color[1] + color.color[1], this->color[2] + color.color[2]};
}

Color Color::multiply(const Color& color) const {
    return {this->color[0] * color.color[0], this->color[1] * color.color[1], this->color[2] * color.color[2]};
}

Color Color::multiply(const double factor) const {
    return multiply({factor, factor, factor});
}

Color Color::clip() const {
    return {
            std::max(0.0, std::min(this->color[0], 1.0)),
            std::max(0.0, std::min(this->color[1], 1.0)),
            std::max(0.0, std::min(this->color[2], 1.0)),
    };
}

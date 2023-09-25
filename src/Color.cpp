#pragma once


#include "Color.h"

Color::Color(float r, float g, float b) {
    std::vector<float> color = {r, g, b};
    this->color = color;

}


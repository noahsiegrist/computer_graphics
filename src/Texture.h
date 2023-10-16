#pragma once

#include "Vec3.h"
#include "Color.h"




class Texture {
public:
    int m_width, m_height, m_channels;
    unsigned char* m_data;
    Texture(const char* filename);

    ~Texture();

    Color getPixelOnSphere(const Vec3& n, const Texture& texture) const;
};


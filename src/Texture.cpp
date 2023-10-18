#include <iostream>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *filename) {
    m_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);


}
Texture::~Texture() {
    stbi_image_free(m_data);
}

Color getColorOn2D(float t, float s, const Texture& texture) {
    if(texture.m_data != nullptr) {
        int s_index = static_cast<int>(s * (texture.m_width - 1));
        int t_index = static_cast<int>(t * (texture.m_height - 1));

        int location = (t_index * texture.m_width + s_index) * texture.m_channels;
        return Color{
                pow(texture.m_data[location] / 255.0f, 2.2f),
                pow(texture.m_data[location + 1] / 255.0f, 2.2f),
                pow(texture.m_data[location + 2] / 255.0f, 2.2f)
        };
    }

    float radial_value = (sin(0.3f+t*30*sin(t*10+0.3f))+1.f)/2.f;
    return Color(radial_value, radial_value, radial_value).multiply(2).multiply(YELLOW);
}

Color Texture::getPixelOnSphere(const Vec3& n, const Texture& texture) const
{
    float s = atan2(n.vec[2], n.vec[0]) / (2.0f * M_PI) + 0.5f;
    float t = acos(n.vec[1]) / M_PI;

    return getColorOn2D(t, s, texture);
}
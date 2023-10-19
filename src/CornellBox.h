#pragma once

#include <vector>
#include "Sphere.h"
#include "ScreenPainter.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;
constexpr int FOV = 110;

class CornellBox {
    Vec3 m_eye;
    Vec3 m_lookAt;

    std::vector<Sphere> m_scene;

    void populateScene();


public:

    CornellBox(const Vec3 &eye, const Vec3 &lookAt);
    void display(ScreenPainter &painter) const;
};



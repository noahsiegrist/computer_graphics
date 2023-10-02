#pragma once

#include <vector>
#include "Sphere.h"
#include "ScreenPainter.h"


class CornellBox {
    const int m_fov;

    Vec3 m_eye;
    Vec3 m_lookAt;

    std::vector<Sphere> m_scene;

    void populateScene();


public:

    CornellBox(int fov, const Vec3 &eye, const Vec3 &lookAt);
    void display(ScreenPainter painter) const;
};



#include <iostream>
#include <utility>
#include <vector>
#include "Sphere.h"
#include "Color.h"
#include "CornellBox.h"
#include "Vec3.h"


std::pair<Sphere, Vec3> findClosestHitPoint(const Vec3& eye, const Vec3& lookAt, const std::vector<Sphere>& scene) {
    const Sphere *closestSphere = &scene[0];
    double shortestDistance = 100000;
    for(int i = 0; i < scene.size(); i++) {
        Vec3 hitPoint = scene[i].intersect(eye, lookAt);
        if(!hitPoint.null && shortestDistance > (hitPoint).length()) {
            shortestDistance = (hitPoint).length();
            closestSphere = &scene[i];
        }
    }
    if(shortestDistance < 100000) {
        return std::make_pair(*closestSphere, closestSphere->intersect(eye, lookAt));
    }
    return std::make_pair(Sphere({0, 0, 0}, 1000, BLACK), Vec3());
}

Vec3 lookAt(const Vec3& eye, const Vec3& lookAt, int x, int y, int screenWidth, int screenHeight, int fov) {
    double aspectRatio = (double)screenWidth / (double)screenHeight;
    double fovRadians = (double)fov * M_PIf / 180.0f;
    double tan = std::tan(fovRadians/ 2.0f);

    double _x = 2.0f * (double)x / (double)screenWidth - 1.f;
    double _y = 2.0f * (double)y / (double)screenWidth - 1.f;
    double cameraX = tan * _x;
    double cameraY = tan * _y / aspectRatio;
    Vec3 lookAtVector = (lookAt - eye).normalize();
    Vec3 right = lookAtVector.cross({0, 1, 0}).normalize();
    Vec3 up = lookAtVector.cross(right).normalize();
    return (lookAtVector - (right * cameraX) + (up * cameraY)).normalize();
}

Color trace(const Vec3& eye, const Vec3& lookAt, const std::vector<Sphere>& scene, int depth) {
    if (depth > 1) {
        return BLACK;
    }
    std::pair<Sphere, Vec3> hitPoint = findClosestHitPoint(eye, lookAt, scene);
    if (hitPoint.second.null) {
        return BLACK;
    }
    return hitPoint.first.color;
//    Vec3 normal = (hitPoint.second - hitPoint.first.center).normalize();
//    Vec3 reflection = (lookAt - normal * 2 * lookAt.dot(normal)).normalize();
//    return hitPoint.first.color.multiply(trace(hitPoint.second, reflection, scene, depth + 1));
}


CornellBox::CornellBox(int fov, const Vec3& eye, const Vec3& lookAt)
    : m_scene()
    , m_eye(eye)
    , m_lookAt(lookAt)
    , m_fov(fov)
{
    populateScene();
}

void CornellBox::populateScene() {
    m_scene.push_back(Sphere({-1001, 0, 0}, 1000, RED, BLACK));
    m_scene.push_back(Sphere({1001, 0, 0}, 1000, BLUE, BLACK));
    m_scene.push_back(Sphere({0, 0, 1001}, 1000, GREY, BLACK));
    m_scene.push_back(Sphere({0, -1001, 0}, 1000, GREY, BLACK));
    m_scene.push_back(Sphere({0, 1001, 0}, 1000, WHITE, WHITE));
    m_scene.push_back(Sphere({-0.6, -0.7, -0.6}, 0.3, YELLOW, BLACK));
    m_scene.push_back(Sphere({0.3, -0.4, 0.3}, 0.6, LIGHTCYAN, BLACK));
};

void CornellBox::display(ScreenPainter painter) const {
    for (int x = 0; x < painter.getScreenWidth(); x++) {
        for (int y = 0; y < painter.getScreenHeight(); y++) {
            Vec3 lookAtVector = lookAt(m_eye, m_lookAt, x, y, painter.getScreenWidth(), painter.getScreenHeight(), m_fov);
            Color color = trace(m_eye, lookAtVector, m_scene, 0);
            painter.setPixel(x, y, color.r(), color.g(), color.b());
        }
    }
}





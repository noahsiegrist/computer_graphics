#include <iostream>
#include <utility>
#include <vector>
#include "Sphere.h"
#include "Color.h"
#include "CornellBox.h"
#include "Vec3.h"


std::pair<Sphere, Vec3> findClosestHitPoint(const Vec3& eye, const Vec3& lookAt, const std::vector<Sphere>& scene) {
    const Sphere *closestSphere = &scene[0];
    double shortestDistance = MAXFLOAT;
    for(int i = 0; i < scene.size(); i++) {
        Vec3 hitPoint = scene[i].intersect(eye, lookAt);
        if(!hitPoint.null && shortestDistance > (hitPoint).length()) {
            shortestDistance = (hitPoint).length();
            closestSphere = &scene[i];
        }
    }
    if(shortestDistance < MAXFLOAT) {
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
Vec3 getRandomVectorCorrected(const Vec3& normal);

Vec3 notLongerThenOne(const Vec3& vec, const Vec3& normal) {
    if(vec.length() > 1) {
        return getRandomVectorCorrected(normal);
    }
    return vec;
}

Vec3 getRandomVectorCorrected(const Vec3& normal) {
    Vec3 random = Vec3::random();
    if (random.normalize().dot(normal) < 0) {
        return notLongerThenOne(random * -1, normal);
    }else {
        return notLongerThenOne(random, normal);
    }
}

namespace {

#include <random>

    std::random_device rd;

// Initialize a Mersenne Twister pseudo-random generator with the seed from the random device
    std::mt19937 gen(rd());

// Create a uniform distribution between -1.0 and 1.0
    std::uniform_real_distribution<> dis(0, 1.0);
}

Color BRDF(Vec3 in, Vec3 out, Vec3 n, Color color) {
    //Vec3 d = in - (n * ((double) 2.f * in.dot(n)));
    if(in.normalize().dot(n.normalize()) > 0.000001) {
        return color;
    }

    return color;//.multiply(1.f/M_PIf);
}

double p = 0.2f;

Color computeColor(const Vec3& eye, const Vec3& lookAt, const std::vector<Sphere>& scene, int depth = 0) {
    std::pair<Sphere, Vec3> hitPoint = findClosestHitPoint(eye, lookAt, scene);
    if (hitPoint.second.null) {
        return BLACK;
    }

    float random_p = dis(gen);
    if (random_p < p) {
        return hitPoint.first.emission;
    }

    const Vec3& in = lookAt;
    const Vec3 normal = (hitPoint.second-hitPoint.first.center).normalize();

    const Vec3 random = getRandomVectorCorrected(normal).normalize();
    const double cosO = random.dot(normal);
    const double factor = 2.0f * M_PI / (1-p);
    Vec3 correctedByOffset = hitPoint.second +normal*0.001;
    Color computeColorRay = computeColor(correctedByOffset, random, scene, depth+1);
    Color sampleColor =
            computeColorRay
            .multiply(BRDF(in, random, normal, hitPoint.first.color))
            .multiply(factor * cosO);
    return hitPoint.first.emission.add(sampleColor).clip();
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
            Color accumulator = BLACK;
            int max = 265;
            for(int i = 0; i < max; i++) {
                accumulator = accumulator.add(computeColor(m_eye, lookAtVector, m_scene));
            }
            Color color = accumulator.multiply(1.f/(float)max);

            painter.setPixel(x, y, color.r(), color.g(), color.b());
            std::cout << x <<  " : " << y << " color: " << color.r() << ", " << color.g() << ", " << color.b() << std::endl;
        }
    }
}





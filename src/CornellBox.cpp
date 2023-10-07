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
        const Vec3& hitPoint = scene[i].intersect(eye, lookAt);
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

Vec3 lookAt(const Vec3& eye, const Vec3& lookAt, int x, int y) {
    constexpr double aspectRatio = (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT;
    constexpr double fovRadians = (double)FOV * M_PIf / 180.0f;

    double _x = 2.0f * (double)x / (double)SCREEN_WIDTH - 1.f;
    double _y = 2.0f * (double)y / (double)SCREEN_HEIGHT - 1.f;
    constexpr double tan = std::tan(fovRadians / 2.0f);
    double cameraX = tan * _x;
    double cameraY = tan * _y / aspectRatio;
    const Vec3& lookAtVector = (lookAt - eye).normalize();
    const Vec3& right = lookAtVector.cross({0, 1, 0}).normalize();
    const Vec3& up = lookAtVector.cross(right).normalize();
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
    const Vec3& random = Vec3::random();
    if (random.normalize().dot(normal) < 0) {
        return notLongerThenOne(random * -1, normal);
    }else {
        return notLongerThenOne(random, normal);
    }
}

namespace {

#include <random>

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0);
}

Color BRDF(const Vec3& in, const Vec3& out, const Vec3& n, const Color& color) {
    const Vec3& specularDirection = in - (n * (2 * in.dot(n)));
    double d = out.normalize().dot(specularDirection.normalize());
    if(d >= 0.9999) {
        return color.multiply(10);
    }

    return color.multiply(1.f/M_PIf);
}

constexpr double p = 0.2f;

Color computeColor(const Vec3& eye, const Vec3& lookAt, const std::vector<Sphere>& scene, int depth = 0) {
    std::pair<Sphere, Vec3> hitPoint = findClosestHitPoint(eye, lookAt, scene);
    if (hitPoint.second.null) {
        return BLACK;
    }

    float random_p = dis(gen);
    if (random_p < p) {
        return hitPoint.first.emission;
    }

    const Vec3& in = eye + hitPoint.second;
    const Vec3& normal = (in-hitPoint.first.center).normalize();

    const Vec3& random = getRandomVectorCorrected(normal).normalize();
    const double cosO = random.dot(normal);
    constexpr double factor = 2.0f * M_PI / (1-p);
    const Vec3& correctedByOffset = in +normal*0.001;
    const Color& computeColorRay = computeColor(correctedByOffset, random, scene, depth+1);
    const Color& sampleColor =
            computeColorRay
            .multiply(BRDF(in, random, normal, hitPoint.first.color))
            .multiply(factor * cosO);
    return hitPoint.first.emission.add(sampleColor);
}


CornellBox::CornellBox(const Vec3& eye, const Vec3& lookAt)
    : m_scene()
    , m_eye(eye)
    , m_lookAt(lookAt)
{
    populateScene();
}

void CornellBox::populateScene() {
    m_scene.push_back(Sphere({-1001, 0, 0}, 1000, RED, BLACK));
    m_scene.push_back(Sphere({1001, 0, 0}, 1000, BLUE, BLACK));
    m_scene.push_back(Sphere({0, 0, 1001}, 1000, GREY, BLACK));
    m_scene.push_back(Sphere({0, -1001, 0}, 1000, GREY, BLACK));
    m_scene.push_back(Sphere({0, 1001, 0}, 1000, WHITE, WHITE.multiply(2)));
    m_scene.push_back(Sphere({-0.6, -0.7, -0.6}, 0.3, YELLOW, BLACK));
    m_scene.push_back(Sphere({0.3, -0.4, 0.3}, 0.6, LIGHTCYAN, BLACK));
};

#include <thread>

void paintPixel(ScreenPainter& painter, int x, int y, const Vec3& eye, const Vec3& ray, const std::vector<Sphere>& scene) {
    const Vec3& lookAtVector = lookAt(eye, ray, x, y);
    Color accumulator = BLACK;
    int max = 4000;
    for(int i = 0; i < max; i++) {
        accumulator = accumulator.add(computeColor(eye, lookAtVector, scene));
    }
    Color color = accumulator.multiply(1.f/(float)max);

    painter.setPixel(x, y, color.r(), color.g(), color.b());
    std::cout << x <<  " : " << y << std::endl; // " color: " << color.r() << ", " << color.g() << ", " << color.b() << std::endl;
}

void CornellBox::display(ScreenPainter painter) const {
    const int batchSize = 100;
    std::cout << "Batch size: " << batchSize << std::endl;
    std::vector<std::thread> threads;

    auto launchThread = [&](int x, int y) {
        threads.push_back(std::thread(paintPixel, std::ref(painter), x, y, m_eye, m_lookAt, m_scene));
    };

    for (int x = 0; x < painter.getScreenWidth(); x++) {
        for (int y = 0; y < painter.getScreenHeight(); y++) {
            // If we've reached the batch size, wait for the first thread to finish
            if (threads.size() >= batchSize) {
                threads.front().join();
                threads.erase(threads.begin());
            }
            launchThread(x, y);
        }
    }

    std::cout << "All threads have been created." << std::endl;
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << "All threads have finished." << std::endl;
}





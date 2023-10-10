#include <iostream>
#include <utility>
#include <vector>
#include "Sphere.h"
#include "Color.h"
#include "CornellBox.h"
#include "Vec3.h"


std::pair<const Sphere*, float> findClosestHitPoint(const Vec3& origin, const Vec3& ray, const std::vector<Sphere>& scene) {
    const Sphere *closestSphere = &scene[0];
    float shortestDistance = MAXFLOAT;
    for(int i = 0; i < scene.size(); i++) {
        const float hitPointLambda = scene[i].intersect(origin, ray);
        if(hitPointLambda != -1 && shortestDistance > hitPointLambda) {
            shortestDistance = hitPointLambda;
            closestSphere = &scene[i];
        }
    }
    return std::make_pair(closestSphere, shortestDistance);
}

Vec3 lookAt(const Vec3& origin, const Vec3& ray, int x, int y) {
    constexpr float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    constexpr float fovRadians = (float)FOV * M_PIf / 180.0f;

    float _x = 2.0f * (float)x / (float)SCREEN_WIDTH - 1.f;
    float _y = 2.0f * (float)y / (float)SCREEN_HEIGHT - 1.f;
    constexpr float tan = std::tan(fovRadians / 2.0f);
    float cameraX = tan * _x;
    float cameraY = tan * _y / aspectRatio;
    const Vec3& lookAtVector = ray.normalize();
    const Vec3& right = lookAtVector.cross({0, 1, 0}).normalize();
    const Vec3& up = lookAtVector.cross(right).normalize();
    return (lookAtVector - (right * cameraX) + (up * cameraY)).normalize();
}

Vec3 getRandomVectorCorrected(const Vec3& normal) {
    const Vec3& random = Vec3::random();
    if(random.length() > 1) {
        return getRandomVectorCorrected(normal);
    }
    if (random.normalize().dot(normal) < 0) {
        return random*-1;
    }
    return random;
}

namespace {

    #include <random>

    std::random_device rd;
    thread_local std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0, 1.0);
}

Color BRDF(const Vec3& in, const Vec3& out, const Vec3& n, const Color& color, bool specular = false) {
    if(specular) {
        const Vec3 &specularDirection = in - (n * (2 * in.dot(n)));
        float d = out.normalize().dot(specularDirection.normalize());
        if (d >= 0.99) {
            return color.multiply(1.f / M_PIf).add(WHITE.multiply(5));
        }
    }
    return color.multiply(1.f/M_PIf);
}

constexpr float p = 0.2f;

Color computeColor(const Vec3& origin, const Vec3& ray, const std::vector<Sphere>& scene, int depth = 0) {
    std::pair<const Sphere*, float> hitPoint = findClosestHitPoint(origin, ray.normalize(), scene);
    if (hitPoint.second == MAXFLOAT) {
        return BLACK;
    }

    float random_p = dis(gen);
    if (random_p < p) {
        return hitPoint.first->emission;
    }

    const Vec3& in = (origin + ray * hitPoint.second);

    const Vec3& normal = (in-hitPoint.first->center).normalize();

    const Vec3& random = getRandomVectorCorrected(normal).normalize();
    const float cosO = random.dot(normal);
    constexpr float factor = 2.0f * M_PI / (1-p);
    const Vec3& correctedByOffset = in +normal*0.0001;
    const Color& computeColorRay = computeColor(correctedByOffset, random, scene, depth+1);

    const Color& sampleColor =
            computeColorRay
            .multiply(BRDF(ray, random, normal, hitPoint.first->color,  hitPoint.first->specular))
            .multiply(factor * cosO);
    const Color& emission = hitPoint.first->emission;

    return emission.add(sampleColor);
}


CornellBox::CornellBox(const Vec3 &eye, const Vec3 &lookAt)
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
    m_scene.push_back(Sphere({-0.6, -0.7, -0.6}, 0.3, YELLOW, BLACK, true));
    m_scene.push_back(Sphere({0.3, -0.4, 0.3}, 0.6, LIGHTCYAN, BLACK, true));
};

#include <thread>

void paintPixel(ScreenPainter &painter, int x, int y, const Vec3& origin, const Vec3& ray, const std::vector<Sphere>& scene) {
    const Vec3& lookAtVector = lookAt(origin, ray, x, y);
    Color accumulator = BLACK;
    int max = 4000;
    for(int i = 0; i < max; i++) {
        accumulator = accumulator.add(computeColor(origin, lookAtVector, scene));
    }
    Color color = accumulator.multiply(1.f/(float)max);

    painter.setPixel(x, y, color.r(), color.g(), color.b());
}

void CornellBox::display(ScreenPainter &painter) const {
    const int batchSize = 100;
    std::cout << "Batch size: " << batchSize << std::endl;
    std::vector<std::thread> threads;

    auto launchThread = [&](int x, int y) {
        threads.push_back(std::thread(paintPixel, std::ref(painter), x, y, m_eye, m_lookAt-m_eye, m_scene));
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
        std::cout << "Finished column " << x << std::endl;
    }

    std::cout << "All threads have been created." << std::endl;
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << "All threads have finished." << std::endl;
}





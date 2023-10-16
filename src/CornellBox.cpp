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

Vec3 lookAt(const Vec3& origin, const Vec3& ray, float x, float y) {
    constexpr float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    constexpr float fovRadians = (float)FOV * M_PIf / 180.0f;

    float _x = 2.0f * x / (float)SCREEN_WIDTH - 1.f;
    float _y = 2.0f * y / (float)SCREEN_HEIGHT - 1.f;
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

Color BRDF(const Vec3& in, const Vec3& out, const Vec3& n, const Sphere* sphere) {
    if(sphere->specular) {
        const Vec3 &specularDirection = in - (n * (2 * in.dot(n)));
        float d = out.normalize().dot(specularDirection.normalize());
        if (d >= 0.99) {
            return sphere->getColor(n).multiply(1.f / M_PIf).add(WHITE.multiply(10));
        }
    }
    return sphere->getColor(n).multiply(1.f/M_PIf);
}

constexpr float p = 0.2f;

Color computeColor(const Vec3& origin, const Vec3& ray, const std::vector<Sphere>& scene, int depth = 0) {
    std::pair<const Sphere*, float> hitPoint = findClosestHitPoint(origin, ray.normalize(), scene);
    if (hitPoint.second == MAXFLOAT) {
        return BLACK;
    }

    float random_p = dis(gen);
    const Vec3& in = (origin + ray * hitPoint.second);
    const Vec3& normal = (in-hitPoint.first->center).normalize();

    if (random_p < p) {
        return hitPoint.first->getEmmission(normal);
    }

    const Vec3& random = getRandomVectorCorrected(normal).normalize();
    const float cosO = random.dot(normal);
    constexpr float factor = 2.0f * M_PI / (1-p);
    const Vec3& correctedByOffset = in +normal*0.0001;
    const Color& computeColorRay = computeColor(correctedByOffset, random, scene, depth+1);

    const Color& sampleColor =
            computeColorRay
            .multiply(BRDF(ray, random, normal, hitPoint.first))
            .multiply(factor * cosO);
    const Color& emission = hitPoint.first->getEmmission(normal);

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
    m_scene.push_back(Sphere({0, 1001, 0}, 1000, WHITE, WHITE.multiply(2), false, true, false));
   // m_scene.push_back(Sphere({-0.6, -0.7, -2}, 0.2, WHITE, WHITE.multiply(10), false, false, false));
    m_scene.push_back(Sphere({-0.6, -0.7, -0.6}, 0.3, YELLOW, BLACK, false, true));
    m_scene.push_back(Sphere({0.3, -0.4, 0.3}, 0.6, LIGHTCYAN, BLACK, true, false, false));
};

template <std::size_t num_samples>
constexpr std::array<float, num_samples> getGausianWeights(const float sigma) {
    std::array<float, num_samples> weights{};
    float weightSum = 0.0f;
    for (int i = 0; i < num_samples; i++) {
        float samplePos = (float(i) / (num_samples - 1)) * 2.0f - 1.0f;
        weights[i] = exp(-samplePos * samplePos / (2.0f * sigma * sigma));
        weightSum += weights[i];
    }
    for (int i = 0; i < num_samples; i++) {
        weights[i] /= weightSum;
    }
    return weights;
}

#include <thread>

void paintPixel(ScreenPainter &painter, int x, int y, const Vec3& origin, const Vec3& ray, const std::vector<Sphere>& scene) {
    const int num_samples = 50;
    const float sigma = 0.5f;

    auto weights = getGausianWeights<num_samples>(sigma);

    Color accumulator = BLACK;

    for (int i = 0; i < num_samples; i++) {
        for (int j = 0; j < num_samples; j++) {
            float offsetX = (float(i) / (num_samples - 1)) - 0.5f;
            float offsetY = (float(j) / (num_samples - 1)) - 0.5f;

            const Vec3& offsetLookAtVector = lookAt(origin, ray, x + offsetX, y + offsetY);
            Color sampleColor = computeColor(origin, offsetLookAtVector, scene);

            accumulator = accumulator.add(sampleColor.multiply(weights[i] * weights[j]));
        }
    }

    painter.setPixel(x, y, accumulator.r(), accumulator.g(), accumulator.b());
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





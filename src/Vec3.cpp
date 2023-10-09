//
// Created by noah on 29.09.23.
//
#include <random>
#include "Vec3.h"

namespace {
    std::random_device rd;
    thread_local std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0, 1.0);
}


Vec3 Vec3::random() {
    return {dis(gen), dis(gen), dis(gen)};
}
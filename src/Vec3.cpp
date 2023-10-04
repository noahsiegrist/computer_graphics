//
// Created by noah on 29.09.23.
//
#include <random>
#include "Vec3.h"

namespace {
std::random_device rd;

// Initialize a Mersenne Twister pseudo-random generator with the seed from the random device
std::mt19937 gen(rd());

// Create a uniform distribution between -1.0 and 1.0
std::uniform_real_distribution<> dis(-1.0, 1.0);

}


Vec3 Vec3::random() {
    return {dis(gen), dis(gen), dis(gen)};
}
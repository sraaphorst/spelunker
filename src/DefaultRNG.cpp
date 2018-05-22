/**
 * DefaultRNG.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <random>

#include "DefaultRNG.h"

namespace vorpal::math {
    DefaultRNG::DefaultRNG()
        : rd(), g(rd()) {}

    int DefaultRNG::randomRangeImpl(int lower, int upper) noexcept {
        std::uniform_int_distribution<> dist(lower, upper-1);
        return dist(g);
    }
}
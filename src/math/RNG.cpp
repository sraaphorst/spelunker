/**
 * RNG.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>

#include "DefaultRNG.h"
#include "RNG.h"

namespace spelunker::math {
    std::shared_ptr<RNG> RNG::rng = nullptr;

    RNG::~RNG() {
        rng.reset();
    }

    void RNG::setRNG(std::shared_ptr<RNG> &nRNG) noexcept {
        if (rng) rng.reset();
        rng = nRNG;
    }

    std::shared_ptr<RNG> RNG::getRNG() noexcept {
        if (!rng) {
            rng = std::make_shared<DefaultRNG>();
        }
        return rng;
    }

    int RNG::randomRange(const int lower, const int upper) {
        if (lower >= upper) {
            const std::string s = std::string("randomRange called with invalid arguments: ") +
                                  "lower = " + std::to_string(lower) +
                                  " upper = " + std::to_string(upper);
            throw std::invalid_argument(s);
        }

        return getRNG()->randomRangeImpl(lower, upper);
    }

    double RNG::randomProbability() {
        return getRNG()->randomProbabilityImpl();
    }
}
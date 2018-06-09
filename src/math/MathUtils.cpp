/**
 * MathUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stdexcept>
#include <string>

#include "MathUtils.h"

namespace spelunker::math {
    void MathUtils::checkProbability(const double probability) {
        if (probability < 0 || probability > 1)
            throw std::invalid_argument("Probability " + std::to_string(probability) + " is not in [0,1].");
    }
}
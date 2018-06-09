/**
 * DefaultRNG.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The default random number generator using a Mersenne twister
 */

#pragma once

#include <random>

#include "RNG.h"

namespace spelunker::math {
    /// The default random number generator, which uses STL's implementation of the Mersenne twister.
    /**
     * The default random number generator, using STL's implementation of the Mersenne twister.
     * If no other RNG is set, this is used by default when accessing the RNG class.
     * No action is required on the part of the user to initialize it.
     */
    class DefaultRNG final : public RNG {
    public:
        DefaultRNG();
        ~DefaultRNG() final = default;

    protected:
        int randomRangeImpl(int lower, int upper) noexcept final;

        double randomProbabilityImpl() noexcept final;

    private:
        std::random_device rd;
        std::mt19937 g;
    };
};

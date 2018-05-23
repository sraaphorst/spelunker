/**
 * DefaultRNG.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The default random number generator using a Mersenne twister
 */

#ifndef SPELUNKER_DEFAULTRNG_H
#define SPELUNKER_DEFAULTRNG_H

#include <random>

#include "RNG.h"

namespace vorpal::math {
    /// The default random number generator, which uses STL's implementation of the Mersenne twister.
    /**
     * The default random number generator, using STL's implementation of the Mersenne twister.
     * If no other RNG is set, this is used by default when accessing the RNG class.
     * No action is required on the part of the user to initialize it.
     */
    class DefaultRNG final : public RNG {
    public:
        DefaultRNG();
        virtual ~DefaultRNG() = default;

    protected:
        virtual int randomRangeImpl(int lower, int upper) noexcept override;

        virtual double randomProbabilityImpl() noexcept override;

    private:
        std::random_device rd;
        std::mt19937 g;
    };
};

#endif //SPELUNKER_DEFAULTRNG_H

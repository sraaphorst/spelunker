/**
 * DefaultRNG.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The default random number generator, a Mersenne twister
 */

#ifndef SPELUNKER_DEFAULTRNG_H
#define SPELUNKER_DEFAULTRNG_H

#include <random>

#include "RNG.h"

namespace vorpal::math {
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

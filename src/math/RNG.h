/**
 * RNG.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Encapsulates a random number generator to allow plugging in different generators.
 */

#include <algorithm>
#include <memory>
#include <stdexcept>

#pragma once

namespace spelunker::math {
    /**
     * The abstract superclass for random number generation algorithms.
     * Implementations can be provided and set. If no implementation is provided,
     * the instance defaults to @see{DefaultRNG}.
     */
    class RNG {
    public:
        RNG() = default;
        virtual ~RNG();

        static void setRNG(std::shared_ptr<RNG> &nRNG) noexcept;
        static std::shared_ptr<RNG> getRNG() noexcept;

        /// Generate a number in the range [lower,upper).
        /**
         * Given a value upper, returns a random number in the range [lower,upper).
         * @param lower the lower bound on the range (inclusive)
         * @param upper the upper bound on the range (exclusive)
         * @return a number r such that lower <= r < upper
         * @throws invalid_argument if lower >= upper
         */
        static int randomRange(int lower, int upper);

        /// Generate a number in the range [0,upper).
        /**
         * Given a value upper, returns a random number in the range [0,upper).
         * @param upper the upper bound on the range (exclusive)
         * @return a number r such that 0 <= r < upper
         * @throws invalid_argument if upper <= 0
         */
        static inline int randomRange(const int upper) { return randomRange(0, upper); }

        /// Generate a random number in the range [0,1).
        /**
         * Generate a random number in the range [0,1).
         * @return a double d such that 0 <= d < 1
         */
        static double randomProbability();

        /// Select a random element from a collection.
        /**
         * Given an STL collection, returns a reference to a random element in it.
         */
        template<typename Container>
        static auto &randomElement(const Container &c);

        /// Shuffle a collection.
        /**
         * Given a collection, use the current RNG to shuffle it.
         * @tparam Container the container type
         * @param c the container of elements to shuffle.
         */
        template<typename Container>
        static void shuffle(Container &c);

    protected:
        /// Generate a number in the range [lower,upper).
        /**
         * Given a range [lower,upper), subclasses of RNG need to generate a number in the range [lower,upper).
         * Note that, when this method is called, RNG has already checked to ensure lower < upper.
         * @param lower lower bound on range (inclusive)
         * @param upper upper bound on range (exclusive)
         * @return a number r such that lower <= r < upper
         */
        virtual int randomRangeImpl(int lower, int upper) noexcept = 0;

        /// Generate a random number in the range [0,1).
        /**
         * Generate a random number in the range [0,1).
         * @return a double d such that 0 <= d < 1
         */
        virtual double randomProbabilityImpl() noexcept = 0;

    private:
        /// The random number generator. RNG takes access to it.
        static std::shared_ptr<RNG> rng;
    };


    template<typename Container>
    auto &RNG::randomElement(const Container &c) {
        auto iter       = c.begin();
        const auto size = std::distance(iter, c.end());
        auto r          = getRNG();
        const auto idx  = r->randomRange(static_cast<int>(size));

        std::advance(iter, idx);
        return *iter;
    }

    template<typename Container>
    void RNG::shuffle(Container &c) {
        auto r = getRNG();

        // c.size is unsigned, so we must cast to int for the case that it is 0.
        const auto maxPos = static_cast<int>(c.size()) - 1;
        for (auto i=0; i < maxPos; ++i) {
            // Find a random element and swap it with begin.
            const auto idx = randomRange(i, c.size());
            std::swap(c[i], c[idx]);
        }
    }
};

/**
 * MathUtils.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Utilities to simplify some mathematical functions and value handling.
 */

#pragma once

namespace spelunker::math {
    /**
     * A class with purely static members to offer handy functions to do with mathematics.
     */
    class MathUtils final {
    private:
        MathUtils() = default;
        ~MathUtils() = default;

    public:
        /**
         * Check to see if a probability is in the valid range [1,0].
         * @param probability the probability to check
         * @throws std::invalid_argument if they are not.
         */
        static void checkProbability(double probability);
    };
}

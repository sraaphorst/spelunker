/**
 * BraidableMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::types {
    template<typename T>
    /**
     * A braided maze is a maze with no dead ends: thus, a BraidableMaze is a maze that implements methods
     * to braid itself, i.e. remove some or all of its dead ends.
     * @tparam T the maze type
     */
    class BraidableMaze {
    public:
        /**
         * A braided maze is a maze with no dead ends.
         * Any maze can be made into a braided maze simply by removing one wall from each dead end.
         *
         * @param probability the probability of eliminating a dead end
         * @return a braided or semi-braided maze
         */
        virtual const T braid(double probability) const noexcept = 0;

        /**
         * A braided maze is a maze with no dead ends.
         * Any maze can be made into a braided maze simply by removing one wall from each dead end.
         *
         * @param probability the probability of eliminating a dead end
         * @return a braided or semi-braided maze
         */
        const T braidAll() const noexcept {
            return braid(1.0);
        }
    };
}
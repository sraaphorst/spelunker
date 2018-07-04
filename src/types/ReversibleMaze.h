/**
 * ReversibleMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Represents a maze type that can reverse itself in some form.
 * Reversing should be an operation of order 2.
 */

#pragma once

namespace spelunker::types {
    /**
     * A maze type that has some concept of being reversible.
     * Reversibility must be an operation of order precisely 2, i.e. a reversed
     * maze must not be equivalent to its original form, but the reverse of a
     * reversible maze must be equivalent to the starting form, with one exception:
     *
     * A newly reversible maze should not have a start cell or goal cells, since they
     * would be reversed into non-cells; thus a doubly reversed maze will continue tp
     * have its start and goal cells eliminated.
     * @tparam T the type of maze
     */
    template<typename T>
    class ReversibleMaze {
    public:
        virtual const T reverse() const noexcept = 0;
    };
}
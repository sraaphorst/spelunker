/**
 * UnicursalizableMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::types {
    /**
     * A unicursal maze - also known as a labyrinth - is a maze where there are no branches.
     *
     * This is a superclass for a maze that has the property that it can be used to produce
     * a unicursal maze.
     *
     * @tparam T the type of the maze, to parameterize the return value
     */
    template<typename T>
    class UnicursalizableMaze {
    public:

        /// From this maze, produce a unicursal maze.
        virtual const T makeUnicursal() const noexcept = 0;
    };
}

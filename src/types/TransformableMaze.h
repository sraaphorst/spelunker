/**
 * TransformableMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "Transformation.h"

namespace spelunker::types {
    /**
     * A maze that can have transformations applied to it to produce a new, isomorphic maze.
     * @tparam T the type of maze
     */
    template<typename T>
    class TransformableMaze {
    public:
        /**
         * Apply a transformation to this maze to get a new one.
         * Reflections in the diagonals can only be done in square mazes.
         * @param t the transformaton to apply
         * @return the transformed maze
         * @throws IllegalGroupOperation if the transformation cannot be applied
         */
        virtual const T applyTransformation(Transformation t) const = 0;
    };
}
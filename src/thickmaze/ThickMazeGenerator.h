/**
 * ThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Superclass of algorithms unique to @see{ThickMaze}s.
 */

#pragma once

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/AbstractMazeGenerator.h>

namespace spelunker::thickmaze {
    class ThickMaze;

    class ThickMazeGenerator : public types::AbstractMazeGenerator<ThickMaze> {
    public:
        ThickMazeGenerator(const types::Dimensions2D &d);
        ThickMazeGenerator(int w, int h);
        virtual ~ThickMazeGenerator() = default;

        virtual const ThickMaze generate() const noexcept = 0;

    protected:
        /**
         * Given a cell c, return its valid neighbours in the maze.
         * @param c the cell
         * @return a list of the cell's neighbours
         */
        const types::CellCollection neighbours(const types::Cell& c) const;
    };
};

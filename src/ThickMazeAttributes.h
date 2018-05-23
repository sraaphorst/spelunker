/**
 * ThickMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes specific to ThickMazes.
 */

#ifndef SPELUNKER_THICKMAZEATTRIBUTES_H
#define SPELUNKER_THICKMAZEATTRIBUTES_H

#include <vector>

namespace spelunker::thickmaze::types {
    /// The different types of contents that a cell may contain.
    enum CellType {
        FLOOR = 0,
        WALL
    };

    /// A container of contents of a ThickMaze, minus the boundary walls.
    using CellContents = std::vector< std::vector< CellType > >;
}

#endif //SPELUNKER_THICKMAZEATTRIBUTES_H

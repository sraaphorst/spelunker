/**
 * ThickMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes specific to ThickMazes.
 */

#pragma once

#include <tuple>
#include <vector>

#include "types/CommonMazeAttributes.h"

namespace spelunker::thickmaze {
    /// The different types of contents that a cell may contain.
    enum CellType {
        FLOOR = 0,
        WALL
    };

    /// A container of contents of a ThickMaze, minus the boundary walls.
    using CellContents = std::vector< std::vector< CellType > >;

    /// Create an empty set of contents (all FLOOR).
    inline CellContents createEmptyThickCellContents(int width, int height) {
        CellContents contents;
        contents.resize(width);
        for (auto i=0; i < width; ++i)
            contents[i].resize(height);
        return contents;
    }
}

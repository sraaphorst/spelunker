/**
 * ThickMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes specific to ThickMazes.
 */

#ifndef SPELUNKER_THICKMAZEATTRIBUTES_H
#define SPELUNKER_THICKMAZEATTRIBUTES_H

#include <tuple>
#include <vector>

#include "CommonMazeAttributes.h"

namespace spelunker::types {
    /// The different types of contents that a cell may contain.
    enum CellType {
        FLOOR = 0,
        WALL
    };

    /// A container of contents of a ThickMaze, minus the boundary walls.
    using CellContents = std::vector< std::vector< CellType > >;

    /// Create an empty set of contents (all FLOOR).
    inline types::CellContents createEmptyThickCellContents(int width, int height) {
        types::CellContents contents;
        contents.resize(width);
        for (auto i=0; i < width; ++i)
            contents[i].resize(height);
        return contents;
    }
}

#endif //SPELUNKER_THICKMAZEATTRIBUTES_H

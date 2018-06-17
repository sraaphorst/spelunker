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

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>

namespace spelunker::thickmaze {
    /// The different types of contents that a cell may contain.
    enum class CellType {
        FLOOR = 0,
        WALL
    };

    /// A container of contents of a ThickMaze, minus the boundary walls.
    using CellRow = std::vector<CellType>;
    using CellContents = std::vector<CellRow>;

    /// Create an empty layout.
    CellContents createThickMazeLayout(int width, int height, CellType c = CellType::FLOOR);

    /// Create an empty layout.
    CellContents createThickMazeLayout(const types::Dimensions2D &d, CellType c = CellType::FLOOR) noexcept;
}

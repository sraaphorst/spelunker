/**
 * ThickMazeAttributes.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include "ThickMazeAttributes.h"

namespace spelunker::thickmaze {
    CellContents createThickMazeLayout(int width, int height, CellType c) {
        return CellContents(width, CellColumnContents(height, c));
    }

    CellContents createThickMazeLayout(const types::Dimensions2D &d, CellType c) noexcept {
        const auto [width, height] = d.values();
        return createThickMazeLayout(width, height, c);
    }
}

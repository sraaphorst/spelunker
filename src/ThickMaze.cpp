/**
 * ThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "Exceptions.h"
#include "ThickMaze.h"

namespace spelunker::thickmaze {
    ThickMaze::ThickMaze(const int w, const int h, const types::CellContents &c)
        : width(w), height(h), contents(c) {
        if (width < 1 || height < 1)
            throw shared::IllegalDimensions(width, height);
    }

    const types::CellType ThickMaze::cell(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw shared::OutOfBoundsCell(x, y);
        return contents[x][y];
    }
}
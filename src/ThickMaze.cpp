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
            throw shared::OutOfBoundsCell(types::cell(x, y));
        return contents[x][y];
    }

    const ThickMaze ThickMaze::reverse() const {
        auto invContents = types::createEmptyThickCellContents(width, height);
        for (auto y=0; y < height; ++y)
            for (auto x=0; x < width; ++x)
                invContents[x][y] = contents[x][y] == types::FLOOR ? types::WALL : types::FLOOR;
        return ThickMaze(width, height, invContents);
    }
}
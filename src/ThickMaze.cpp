/**
 * ThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "Exceptions.h"
#include "ThickMaze.h"

namespace spelunker::thickmaze {
    ThickMaze::ThickMaze(const int w, const int h, const CellContents &c)
        : width(w), height(h), contents(c) {
        if (width < 1 || height < 1)
            throw IllegalDimensions(width, height);
    }

    const CellType ThickMaze::cellIs(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw OutOfBoundsCell(cell(x, y));
        return contents[x][y];
    }

    const ThickMaze ThickMaze::reverse() const {
        auto invContents = createEmptyThickCellContents(width, height);
        for (auto y=0; y < height; ++y)
            for (auto x=0; x < width; ++x)
                invContents[x][y] = contents[x][y] == FLOOR ? WALL : FLOOR;
        return ThickMaze(width, height, invContents);
    }
}
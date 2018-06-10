/**
 * ThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <tuple>

#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    ThickMazeGenerator::ThickMazeGenerator(int w, int h)
            : width(w), height(h) {}


    const types::CellCollection ThickMazeGenerator::neighbours(const types::Cell &c) {
        types::CellCollection nbrs;
        const auto [x,y] = c;

        if (x > 0)          nbrs.emplace_back(std::make_pair(x-1,   y));
        if (x < width - 1)  nbrs.emplace_back(std::make_pair(x+1,   y));
        if (y > 0)          nbrs.emplace_back(std::make_pair(  x, y-1));
        if (y < height - 1) nbrs.emplace_back(std::make_pair(  x, y+1));

        return nbrs;
    }
}
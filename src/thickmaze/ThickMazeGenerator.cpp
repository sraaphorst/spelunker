/**
 * ThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <tuple>

#include <types/AbstractMazeGenerator.h>
#include <types/Dimensions2D.h>

#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    ThickMazeGenerator::ThickMazeGenerator(const types::Dimensions2D &d)
        : AbstractMazeGenerator{d} {}

    ThickMazeGenerator::ThickMazeGenerator(int w, int h)
        :  ThickMazeGenerator{types::Dimensions2D{w, h}} {}


    const types::CellCollection ThickMazeGenerator::neighbours(const types::Cell &c) const {
        getDimensions().checkCell(c);

        types::CellCollection nbrs;
        const auto [x,y] = c;

        if (x > 0)               nbrs.emplace_back(std::make_pair(x-1,   y));
        if (x < getWidth() - 1)  nbrs.emplace_back(std::make_pair(x+1,   y));
        if (y > 0)               nbrs.emplace_back(std::make_pair(  x, y-1));
        if (y < getHeight() - 1) nbrs.emplace_back(std::make_pair(  x, y+1));

        return nbrs;
    }
}
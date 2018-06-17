/**
 * CommonMazeAttributes.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "CommonMazeAttributes.h"
#include "Dimensions2D.h"
#include "Direction.h"

namespace spelunker::types {
    Direction cellDirection(const Cell &c1, const Cell &c2) {
        auto [x1, y1] = c1;
        auto [x2, y2] = c2;

        if (x1 - x2 == -1 && y1 == y2)
            return Direction::EAST;
        if (x1 - x2 ==  1 && y1 == y2)
            return Direction::WEST;
        if (y1 - y2 == -1 && x1 == x2)
            return Direction::SOUTH;
        if (y1 - y2 ==  1 && x1 == x2)
            return Direction::NORTH;

        // If we reach this point, the cells aren't adjacent.
        throw std::domain_error("Vertices are not adjacent");
    }

    CellIndicator initializeCellIndicator(int width, int height, bool def) {
        return initializeCellIndicator(Dimensions2D{width, height}, def);
    }

    CellIndicator initializeCellIndicator(const Dimensions2D &d, bool def) {
        const auto [width, height] = d.values();
        return CellIndicator(width, CellRowIndicator(height, def));
    }
}
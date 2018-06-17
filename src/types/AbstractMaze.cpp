/**
 * AbstractMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <optional>

#include "CommonMazeAttributes.h"
#include "Dimensions2D.h"
#include "Exceptions.h"
#include "AbstractMaze.h"

namespace spelunker::types {
    AbstractMaze::AbstractMaze(const Dimensions2D &d, const PossibleCell &startPos, const CellCollection &endPos)
        : dimensions{d}, startCell{startPos}, goalCells{endPos} {
        if (d.getWidth() < 1 || d.getHeight() < 1)
            throw types::IllegalDimensions(d);
        if (startCell)
            checkCell(*startCell);
        for (const auto g: goalCells)
            checkCell(g);
    }

    AbstractMaze::AbstractMaze(const Dimensions2D &d)
        : AbstractMaze{d, {}, CellCollection()} {}

    AbstractMaze::AbstractMaze(const int w, const int h, const PossibleCell &startPos, const CellCollection &endPos)
        : AbstractMaze{Dimensions2D{w,h}, startPos, endPos} {}

    AbstractMaze::AbstractMaze(int w, int h)
        : AbstractMaze{Dimensions2D{w, h}, {}, CellCollection()} {}

    void AbstractMaze::setStartingCell(const PossibleCell &startPos) {
        if (startPos)
            checkCell(*startPos);
        startCell = startPos;
    }

    void AbstractMaze::setGoalCells(const CellCollection &cells) {
        std::for_each(cells.begin(), cells.end(), checkCell);
        goalCells = cells;
    }

    void AbstractMaze::checkCell(const Cell &c) const {
        dimensions.checkCell(c);
    }

    void AbstractMaze::checkCell(const int x, const int y) const {
        dimensions.checkCell(x, y);
    }

    bool AbstractMaze::cellInBounds(const Cell &c) const {
        const auto [x,y] = c;
        return cellInBounds(x, y);
    }

    bool AbstractMaze::cellInBounds(int x, int y) const {
        return dimensions.cellInBounds(x, y);
    }
}
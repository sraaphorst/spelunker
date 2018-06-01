/**
 * ThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "math/MathUtils.h"
#include "math/RNG.h"
#include "types/CommonMazeAttributes.h"
#include "types/Exceptions.h"
#include "ThickMaze.h"

namespace spelunker::thickmaze {
    ThickMaze::ThickMaze(const int w, const int h, const CellContents &c)
        : width(w), height(h), contents(c) {
        if (width < 1 || height < 1)
            throw types::IllegalDimensions(width, height);
    }

    const CellType ThickMaze::cellIs(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw types::OutOfBoundsCell(types::cell(x, y));
        return contents[x][y];
    }

    int ThickMaze::numCellWalls(const types::Cell &c) const {
        return numCellWallsInContents(c, contents);
    }

    const ThickMaze ThickMaze::reverse() const {
        auto invContents = createEmptyThickCellContents(width, height);
        for (auto y=0; y < height; ++y)
            for (auto x=0; x < width; ++x)
                invContents[x][y] = contents[x][y] == FLOOR ? WALL : FLOOR;
        return ThickMaze(width, height, invContents);
    }

    const ThickMaze ThickMaze::braid(double probability) const {
        math::MathUtils::checkProbability(probability);

        // Create a copy of the contents for this maze for the new maze.
        CellContents newContents = contents;

        // Find all the dead ends and store them, shuffle them, and process.
        types::CellCollection deadends = findDeadEnds();
        math::RNG::shuffle(deadends);

        // Lambda function to determine if a cell is a dead end.
        auto isDeadEnd = [&newContents, this](const int x, const int y) {
            if (x < 0 || x >= width || y < 0 || y >= height || newContents[x][y] == WALL)
                return false;
            return numCellWalls(types::cell(x,y)) == 3;
        };

        // Lambda function to determine how many dead ends are around a wall.
        // Returns -1 if the coordinates do not specify a valid wall.
        auto deadEndCounter = [&newContents, isDeadEnd, this](const int x, const int y) {
            if (x < 0 || x >= width || y < 0 || y >= width || newContents[x][y] != WALL)
                return -1;

            int numDeadEnds = 0;
            if (isDeadEnd(x-1, y)) ++numDeadEnds;
            if (isDeadEnd(x+1, y)) ++numDeadEnds;
            if (isDeadEnd(x, y-1)) ++numDeadEnds;
            if (isDeadEnd(x, y+1)) ++numDeadEnds;
            return numDeadEnds;
        };

        for (auto c: deadends) {
            // Check that the probability successds and that thus cell is still a dead end.
            if (math::RNG::randomProbability() > probability || numCellWallsInContents(c, newContents) < 3) {
                continue;
            }
            const auto [x,y] = c;

            // Create a list of the cells around this cell that can be removed and that
            // eliminate the largest number of other dead ends.
            std::vector<types::Cell> candidates;
            int maxDeadEnds = 0;

            auto cellHandler = [&maxDeadEnds, &candidates, deadEndCounter](const int x, const int y) {
                auto dw = deadEndCounter(x, y);
                if (dw < maxDeadEnds)
                    return;

                if (dw > maxDeadEnds) {
                    candidates.clear();
                    maxDeadEnds = dw;
                }
                candidates.emplace_back(types::cell(x,y));
            };

            cellHandler(x-1, y);
            cellHandler(x+1, y);
            cellHandler(x, y-1);
            cellHandler(x, y+1);

            // Now pick a candidate and remove the wall.
            const auto [ex, ey] = math::RNG::randomElement(candidates);
            newContents[ex][ey] = FLOOR;
        }

        return ThickMaze(width, height, newContents);
    }

    const types::CellCollection ThickMaze::findDeadEnds() const noexcept {
        types::CellCollection deadends;
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                const auto c = types::cell(x, y);
                if (numCellWalls(c) == 3)
                    deadends.emplace_back(c);
            }
        }
        return deadends;
    }

    int ThickMaze::numCellWallsInContents(const types::Cell &c, const CellContents &cc) const {
        checkCell(c);
        const auto [x,y] = c;
        auto numCellWalls = 0;

        if (x == 0 || cc[x-1][y] == WALL)        ++numCellWalls;
        if (x == width-1 || cc[x+1][y] == WALL)  ++numCellWalls;
        if (y == 0 || cc[x][y-1] == WALL)        ++numCellWalls;
        if (y == height-1 || cc[x][y+1] == WALL) ++numCellWalls;

        return numCellWalls;
    }


    void ThickMaze::checkCell(const types::Cell &c) const {
        const auto [x,y] = c;
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw types::OutOfBoundsCell(types::Cell(x, y));
    }
}
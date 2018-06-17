/**
 * AldousBroderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "AldousBroderMazeGenerator.h"

namespace spelunker::maze {
    AldousBroderMazeGenerator::AldousBroderMazeGenerator(const spelunker::types::Dimensions2D &d)
        : MazeGenerator{d} {}

    AldousBroderMazeGenerator::AldousBroderMazeGenerator(int w, int h)
        : AldousBroderMazeGenerator{types::Dimensions2D{w, h}} {}

    const Maze AldousBroderMazeGenerator::generate() const {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Keep track of which cells have and have not been visited.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a random starting position.
        auto currX = spelunker::math::RNG::randomRange(width);
        auto currY = spelunker::math::RNG::randomRange(height);
        ci[currX][currY] = true;

        // Keep track of the number of cells visited so we know when to stop.
        auto maxCells = width * height;
        int numCellsVisited = 1;

        while (numCellsVisited < maxCells) {
            // Get all the neighbours of the current cell and move to one at random.
            const auto nbrs = allNeighbours(types::cell(currX, currY));
            const auto nbr  = math::RNG::randomElement(nbrs);
            currX = nbr.first.first;
            currY = nbr.first.second;

            // Process this cell.
            if (!ci[currX][currY]) {
                ci[currX][currY] = true;
                wi[rankPos(nbr)] = false;
                ++numCellsVisited;
            }
        }

        return Maze(getDimensions(), wi);
    }
}
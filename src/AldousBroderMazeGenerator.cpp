/**
 * AldousBroderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "AldousBroderMazeGenerator.h"

namespace spelunker::maze {
    AldousBroderMazeGenerator::AldousBroderMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze AldousBroderMazeGenerator::generate() {
        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Keep track of which cells have and have not been visited.
        CellIndicator ci(width, CellRowIndicator(height, false));

        // Pick a random starting position.
        auto currX = spelunker::math::RNG::randomRange(width);
        auto currY = spelunker::math::RNG::randomRange(height);
        ci[currX][currY] = true;

        // Keep track of the number of cells visited so we know when to stop.
        auto maxCells = width * height;
        int numCellsVisited = 1;

        while (numCellsVisited < maxCells) {
            // Get all the neighbours of the current cell and move to one at random.
            const auto nbrs = allNeighbours(cell(currX, currY));
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

        return Maze(width, height, wi);
    }
}
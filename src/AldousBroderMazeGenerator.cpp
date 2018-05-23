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

namespace vorpal::maze {
    AldousBroderMazeGenerator::AldousBroderMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze AldousBroderMazeGenerator::generate() {
        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Keep track of which cells have and have not been visited.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a random starting position.
        auto currX = vorpal::math::RNG::randomRange(width);
        auto currY = vorpal::math::RNG::randomRange(height);
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
            const auto d = nbr.second;

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
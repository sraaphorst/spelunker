/**
 * HuntAndKillMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "types/CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "math/RNG.h"
#include "HuntAndKillMazeGenerator.h"

#include <iostream>
using namespace std;

namespace spelunker::maze {
    HuntAndKillMazeGenerator::HuntAndKillMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze HuntAndKillMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Get a starting cell.
        auto currX = math::RNG::randomRange(width);
        auto currY = math::RNG::randomRange(height);

        // We keep track of where to search when we start fresh.
        auto nextX = 0;
        auto nextY = 0;

        while (currX < width && currY < height) {
            randomPathCarving(currX, currY, ci, wi);

            // Start scanning back at top, since we may have passed cells we were not
            // adjacent to but now are.
            nextX = 0;
            nextY = 0;

            while (nextX < width && nextY < height) {
                // If unvisited, check to see if visited nbrs.
                // If a visited nbr, break out of this loop.
                if (!ci[nextX][nextY]) {
                    const auto nbrs = visitedNeighbours(types::cell(nextX, nextY), ci);
                    if (!nbrs.empty()) {
                        const auto nbr = math::RNG::randomElement(nbrs);
                        wi[rankPos(nbr)] = false;
                        break;
                    }
                }
                const int newX = nextX + 1;
                nextX = newX % width;
                nextY += newX / width;
            }

            currX = nextX;
            currY = nextY;
        }

        return Maze(width, height, wi);
    }

    void HuntAndKillMazeGenerator::randomPathCarving(const int startX,
                                                     const int startY,
                                                     types::CellIndicator &ci,
                                                     WallIncidence &wi) const noexcept  {
        int x = startX;
        int y = startY;

        for (;;) {
            // Mark the cell as visited.
            ci[x][y] = true;
            
            // Continuously carve to an adjacent unvisited cell.
            const auto nbrs = unvisitedNeighbours(types::cell(x, y), ci);
            if (nbrs.empty()) break;
            const auto nbr = math::RNG::randomElement(nbrs);
            wi[rankPos(nbr)] = false;
            x = nbr.first.first;
            y = nbr.first.second;
        }
    }
}
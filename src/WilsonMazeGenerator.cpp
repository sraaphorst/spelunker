/**
 * WilsonMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <map>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "WilsonMazeGenerator.h"

namespace spelunker::maze {
    WilsonMazeGenerator::WilsonMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze WilsonMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check which cells are part of the maze.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a starting cell at random and add it to the maze.
        const int startX = math::RNG::randomRange(width);
        const int startY = math::RNG::randomRange(height);
        ci[startX][startY] = true;

        // Create a list of all cells and shuffle them to use as the
        // starting positions for random walks.
        std::vector<int> cellRks;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y) {
                if (x == startX && y == startY)
                    continue;
                cellRks.emplace_back(rankCell(x, y));
            }
        math::RNG::shuffle(cellRks);

        // Try to start a new random walk at cellRk.
        for (auto cellRk : cellRks) {
            // If we have already visited this cell, skip it.
            const auto startCell = unrankCell(cellRk);
            auto x = startCell.first;
            auto y = startCell.second;
            if (ci[x][y])
                continue;

            // Generate a random walk. We do this by continuously choosing a
            // neighbour and storing the position we entered this neighbour.
            // prevcell and x and y should always represent the previous cell.
            std::map<int, types::Direction> walk;
            for (;;) {
                // Find the neighbours and pick one at random.
                auto nbrs = allNeighbours(types::cell(x, y));
                auto nbr = math::RNG::randomElement(nbrs);

                // Now update the map to say we reached nbr by the given
                // position from prevCell.
                const auto [nextCell, dir] = nbr;

                // We flip the direction so we are coming into nextCell instead of leaving it.
                walk[rankCell(x, y)] = types::flip(dir);

                // Advance in our walk.
                x = nextCell.first;
                y = nextCell.second;

                // If we have reached a visited cell, terminate our walk.
                if (ci[x][y])
                    break;
            }

            // Now start at the starting cell, and continue following the walk in the
            // accumulated directions until we reach a cell in the maze.
            // Note this will likely not be all the cells in the walk, due to the
            // direction update. This is to prevent loops.
            x = startCell.first;
            y = startCell.second;
            for (; !ci[x][y];) {
                // Mark c as visited, remove the wall, and find the next cell.
                ci[x][y] = true;
                auto rk = rankCell(x, y);
                const auto dir = walk[rk];
                wi[rankPos(types::pos(x, y, dir))] = false;

                switch (dir) {
                    case types::NORTH:
                        y -= 1;
                        break;
                    case types::EAST:
                        x += 1;
                        break;
                    case types::SOUTH:
                        y += 1;
                        break;
                    case types::WEST:
                        x -= 1;
                        break;
                }
            }
        }

        return Maze(width, height, wi);
    }
}
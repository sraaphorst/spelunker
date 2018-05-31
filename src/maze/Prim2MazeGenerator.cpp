/**
 * RandomizedPrim2MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>

#include "types/CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "math/RNG.h"
#include "Prim2MazeGenerator.h"

namespace spelunker::maze {
    Prim2MazeGenerator::Prim2MazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze Prim2MazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a start cell at random.
        const int startX = math::RNG::randomRange(width);
        const int startY = math::RNG::randomRange(height);

        // Create a list of cells. We add the unvisited cells adjacent to cells
        // we have visited to it.
        ci[startX][startY] = true;
        types::CellCollection cells;
        addUnivisitedNeighbourCells(types::cell(startX, startY), cells, ci);

        while (!cells.empty()) {
            // Pick a random cell from the list.
            // Swap it with the end element and remove it for efficiency.
            const int cellIdx = math::RNG::randomRange(cells.size());
            std::swap(cells[cellIdx], cells.back());
            const auto cell = cells.back();
            cells.pop_back();

            // If we have already visited this cell, continue.
            if (ci[cell.first][cell.second])
                continue;

            // Get the visited neighbours of this node.
            // If there are none, then this is the first pass, so use unvisited neighbours.
            const auto nbrs = visitedNeighbours(cell, ci);
            if (nbrs.empty()) continue;

            // Pick one at random, remove the wall between them, mark the cell as visited.
            const auto nbr = math::RNG::randomElement(nbrs);

            const auto wallID = rankPos(nbr);
            wi[wallID] = false;
            ci[cell.first][cell.second] = true;

            // Add the unvisited neighbour cells of the cell to the collection.
            addUnivisitedNeighbourCells(cell, cells, ci);
        }

        return Maze(width, height, wi);
    }

    void Prim2MazeGenerator::addUnivisitedNeighbourCells(const types::Cell &c,
                                                         types::CellCollection &cells,
                                                         const types::CellIndicator &ci) {
        const auto[x, y] = c;
        if (x - 1 >= 0 && !ci[x - 1][y])
            cells.emplace_back(types::cell(x - 1, y));
        if (x + 1 < width && !ci[x + 1][y])
            cells.emplace_back(types::cell(x + 1, y));
        if (y - 1 >= 0 && !ci[x][y - 1])
            cells.emplace_back(types::cell(x, y - 1));
        if (y + 1 < height && !ci[x][y + 1])
            cells.emplace_back(types::cell(x, y + 1));
    }
}
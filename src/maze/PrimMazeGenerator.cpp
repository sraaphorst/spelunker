/**
 * RandomizedPrimMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "types/CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "math/RNG.h"
#include "PrimMazeGenerator.h"

namespace spelunker::maze {
    PrimMazeGenerator::PrimMazeGenerator(int w, int h)
        : MazeGenerator(w, h) {}

    const Maze PrimMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a cell at random.
        const int startX = math::RNG::randomRange(width);
        const int startY = math::RNG::randomRange(height);

        // Create a wall list, add the walls of the start cell to it, and mark
        // the start cell as visited.
        WallCollection walls;
        addCellWalls(types::cell(startX, startY), walls, wi);
        ci[startX][startY] = true;
        // We also need to be able to unrank walls.
        auto unrank = createUnrankWallMap();

        while (!walls.empty()) {
            // Pick a random wall from the list.
            // We swap it with the end element and remove that for efficiency.
            const auto wallIdx = math::RNG::randomRange(walls.size());
            std::swap(walls[wallIdx], walls.back());
            const auto wallID = walls.back();
            walls.pop_back();

            // This wall divides two cells: at most one of them will be unvisited.
            const auto  cells = unrank[wallID];
            const auto &cell1 = cells.first.first;
            const auto &cell2 = cells.second.first;

            const auto cell1Visited = ci[cell1.first][cell1.second];
            const auto cell2Visited = ci[cell2.first][cell2.second];

            if (cell1Visited && cell2Visited)
                continue;
            const auto &unvisitedCell = cell1Visited ? cell2 : cell1;

            // Remove this wall, mark the cell as visited, and add its walls to the list.
            wi[wallID] = false;
            ci[unvisitedCell.first][unvisitedCell.second] = true;
            addCellWalls(unvisitedCell, walls, wi);
        }

        return Maze(width, height, wi);
    }

    void PrimMazeGenerator::addCellWalls(const types::Cell &c,
                                         WallCollection &wallList,
                                         const WallIncidence &wi) {
        // Check each of the four walls to make sure they are valid and not a boundary wall.
        const auto [x, y] = c;

        // Convenience method to add existing walls to the list.
        auto adder = [this, &wi, &wallList](const int nx, const int ny, const types::Direction d) {
            const WallID rk = rankPos(types::pos(nx, ny, d));
            if (rk >= 0 && wi[rk]) wallList.emplace_back(rk);
        };

        // To avoid unnecessary unranking, check we aren't trying to add a boundary wall here.
        if (x - 1 >= 0)
            adder(x, y, types::WEST);
        if (x + 1 < width)
            adder(x, y, types::EAST);
        if (y - 1 >= 0)
            adder(x, y, types::NORTH);
        if (y + 1 < height)
            adder(x, y, types::SOUTH);
    }
}
/**
 * BFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <queue>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "BFSMazeGenerator.h"

namespace spelunker::maze {
    BFSMazeGenerator::BFSMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {};

    const Maze BFSMazeGenerator::generate() {
        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Pick a random starting cell, mark it, and add its neighbours to a queue.
        const auto startX = math::RNG::randomRange(width);
        const auto startY = math::RNG::randomRange(height);
        ci[startX][startY] = true;

        std::queue<types::Cell> queue;
        const auto startNbrs = unvisitedNeighbours(types::cell(startX, startY), ci);
        for (auto startNbr : startNbrs)
            queue.push(startNbr.first);

        while (!queue.empty()) {
            // Pick the front cell from the queue. If it has somehow been visited,
            // then skip it.
            const auto cell = queue.front();
            queue.pop();
            if (ci[cell.first][cell.second]) continue;

            // Otherwise, find its visited neighbours in the maze and pick one at random.
            const auto vnbrs = visitedNeighbours(cell, ci);
            const auto vnbr  = math::RNG::randomElement(vnbrs);
            wi[rankPos(vnbr)] = false;
            ci[cell.first][cell.second] = true;

            // Add the unvisited neighbours to the queue.
            const auto unbrs = unvisitedNeighbours(cell, ci);
            for (auto unbr : unbrs)
                queue.push(unbr.first);
        }

        return Maze(width, height, wi);
    }
}
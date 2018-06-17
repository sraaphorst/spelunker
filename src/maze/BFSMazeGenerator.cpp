/**
 * BFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <queue>

#include <types/CommonMazeAttributes.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "BFSMazeGenerator.h"

namespace spelunker::maze {
    BFSMazeGenerator::BFSMazeGenerator(const types::Dimensions2D &d)
        : MazeGenerator{d} {}

    BFSMazeGenerator::BFSMazeGenerator(int w, int h)
        : BFSMazeGenerator{types::Dimensions2D{w, h}} {}

    const Maze BFSMazeGenerator::generate() const noexcept {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and remove them iteratively.
        auto wi = createMazeLayout(getDimensions(), true);

        // We need a cell lookup to check if we have visited a cell already.
        auto ci = types::initializeCellIndicator(getDimensions(), false);

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

            const auto [cellx, celly] = cell;
            if (ci[cellx][celly]) continue;

            // Otherwise, find its visited neighbours in the maze and pick one at random.
            const auto vnbrs = visitedNeighbours(cell, ci);
            const auto vnbr  = math::RNG::randomElement(vnbrs);
            wi[rankPos(vnbr)] = false;
            ci[cellx][celly] = true;

            // Add the unvisited neighbours to the queue.
            const auto unbrs = unvisitedNeighbours(cell, ci);
            for (auto unbr : unbrs)
                queue.push(unbr.first);
        }

        return Maze(getDimensions(), wi);
    }
}
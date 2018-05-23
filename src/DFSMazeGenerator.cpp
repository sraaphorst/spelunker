/**
 * RandomizedDFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stack>
#include <vector>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "DFSMazeGenerator.h"

namespace spelunker::maze {
    DFSMazeGenerator::DFSMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze DFSMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Create the stack and pick a starting cell.
        std::stack<types::Cell> stack;
        stack.push(types::cell(math::RNG::randomRange(width), math::RNG::randomRange(height)));

        while (!stack.empty()) {
            // Marking c visited will occur multiple times, but we don't care.
            const auto c = stack.top();
            ci[c.first][c.second] = true;

            // Find a list of unvisited neighbours. If we can't find one, then backtrack.
            const auto nbrs = unvisitedNeighbours(c, ci);
            if (nbrs.empty()) {
                stack.pop();
                continue;
            }

            // Pick an unvisited neighbour, remove the wall to it, and move to it by pushing it on the stack.
            const auto nbr = math::RNG::randomElement(nbrs);
            const auto w = rankPos(nbr);
            wi[w] = false;
            stack.push(nbr.first);
        }

        return Maze(width, height, wi);
    }
}
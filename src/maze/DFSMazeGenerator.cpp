/**
 * RandomizedDFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stack>
#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "DFSMazeGenerator.h"

namespace spelunker::maze {
    DFSMazeGenerator::DFSMazeGenerator(const types::Dimensions2D &d)
        : MazeGenerator{d} {}

    DFSMazeGenerator::DFSMazeGenerator(int w, int h)
        : MazeGenerator{types::Dimensions2D{w, h}} {}

    const Maze DFSMazeGenerator::generate() const noexcept {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and then remove them iteratively.
        auto wi = createMazeLayout(getDimensions(), true);

        // We need a cell lookup to check if we have visited a cell already.
        auto ci = types::initializeCellIndicator(getDimensions(), false);

        // Create the stack and pick a starting cell.
        std::stack<types::Cell> stack;
        stack.push(types::cell(math::RNG::randomRange(width), math::RNG::randomRange(height)));

        while (!stack.empty()) {
            // Marking c visited will occur multiple times, but we don't care.
            const auto c = stack.top();
            const auto [cx, cy] = c;
            ci[cx][cy] = true;

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

        return Maze(getDimensions(), wi);
    }
}
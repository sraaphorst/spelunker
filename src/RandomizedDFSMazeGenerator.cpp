/**
 * RandomizedDFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <random>
#include <stack>
#include <vector>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RandomizedDFSMazeGenerator.h"

namespace vorpal::maze {
    RandomizedDFSMazeGenerator::RandomizedDFSMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze RandomizedDFSMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        CellIndicator ci(width, CellRowIndicator(height, false));

        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<> rowDis(0, width - 1);
        std::uniform_int_distribution<> colDis(0, height - 1);

        // Create the stack and pick a starting cell.
        std::stack<types::Cell> stack;
        stack.push(types::cell(rowDis(g), colDis(g)));

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
            std::uniform_int_distribution<> dis(0, nbrs.size() - 1);
            const auto nbr = nbrs[dis(g)];
            const auto w = rankPos(nbr);
            wi[w] = false;
            stack.push(nbr.first);
        }

        return Maze(width, height, wi);
    }

    const RandomizedDFSMazeGenerator::Neighbours
    RandomizedDFSMazeGenerator::unvisitedNeighbours(const types::Cell &c, const CellIndicator &ci) const {
        Neighbours nbrs;

        const int x = c.first;
        const int y = c.second;

        // Note we have to SWITCH the directions here because we want the direction in which we come INTO that node.
        if (x - 1 >= 0 && !ci[x - 1][y])
            nbrs.emplace_back(types::pos(x - 1, y, types::EAST));
        if (y - 1 >= 0 && !ci[x][y - 1])
            nbrs.emplace_back(types::pos(x, y - 1, types::SOUTH));
        if (x + 1 < width && !ci[x + 1][y])
            nbrs.emplace_back(types::pos(x + 1, y, types::WEST));
        if (y + 1 < height && !ci[x][y + 1])
            nbrs.emplace_back(types::pos(x, y + 1, types::NORTH));
        return nbrs;
    }
}
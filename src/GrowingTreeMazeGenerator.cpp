/**
 * GrowingTreeMazeGeneration.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stdexcept>
#include <string>

#include "CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "GrowingTreeMazeGenerator.h"

namespace spelunker::maze {
    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const int w, const int h, const CellSelectionStrategy &strategy)
            : GrowingTreeMazeGenerator(w, h, getSelector(strategy)) {}

    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const int w, const int h, Selector sel)
            : MazeGenerator(w, h), selector(sel) {}

    const Maze GrowingTreeMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // We need a cell lookup to check if we have visited a cell already.
        types::CellIndicator ci(width, types::CellRowIndicator(height, false));

        // Create the cell collection and pick a starting cell.
        types::CellCollection C;
        C.emplace_back(types::cell(math::RNG::randomRange(width), math::RNG::randomRange(height)));

        while (!C.empty()) {
            // Select the element c using the selector.
            const auto idx = selector(C);
            if (idx < 0 || idx >= C.size())
                throw std::out_of_range("C has size " + std::to_string(C.size()) + ", selector tried to pick " + std::to_string(idx));

            const auto c = C[idx];
            ci[c.first][c.second] = true;

            // Find a list of unvisited neighbours. If we can't find one, continue.
            const auto nbrs = unvisitedNeighbours(c, ci);
            if (nbrs.empty()) {
                C.erase(C.begin() + idx);
                continue;
            }

            // Pick an unvisited neighbour, remove the wall to it, and add it to C.
            const auto nbr = math::RNG::randomElement(nbrs);
            auto [nbrx, nbry] = nbr.first;
            ci[nbrx][nbry] = true;
            wi[rankPos(nbr)] = false;
            C.emplace_back(nbr.first);
        }

        return Maze(width, height, wi);
    }

    GrowingTreeMazeGenerator::Selector GrowingTreeMazeGenerator::getSelector(const CellSelectionStrategy css) {
        switch (css) {
            case OLDEST: return [](const types::CellCollection &c) { return 0; };
            case NEWEST: return [](const types::CellCollection &c) { return c.size() - 1; };
            case MIDDLE: return [](const types::CellCollection &c) { return c.size() / 2; };
            case RANDOM: return [](const types::CellCollection &c) { return math::RNG::randomRange(c.size()); };
        }
    }
}
/**
 * GrowingTreeMazeGeneration.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stdexcept>
#include <string>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "GrowingTreeMazeGenerator.h"

namespace spelunker::maze {
    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const types::Dimensions2D &d, const CellSelectionStrategy &sel)
        : GrowingTreeMazeGenerator{d, getSelector(sel)} {}

    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const int w, const int h, const CellSelectionStrategy &sel)
        : GrowingTreeMazeGenerator{types::Dimensions2D{w, h}, getSelector(sel)} {}

    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const types::Dimensions2D &d, Selector sel)
        : MazeGenerator{d}, selector{sel} {}

    GrowingTreeMazeGenerator::GrowingTreeMazeGenerator(const int w, const int h, Selector sel)
        : GrowingTreeMazeGenerator{types::Dimensions2D{w, h}, sel} {}

    const Maze GrowingTreeMazeGenerator::generate() const noexcept {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and then remove them iteratively.
        auto wi = createMazeLayout(getDimensions(), true);

        // We need a cell lookup to check if we have visited a cell already.
        auto ci = types::initializeCellIndicator(getDimensions(), false);

        // Create the cell collection and pick a starting cell.
        types::CellCollection C;
        C.emplace_back(types::cell(math::RNG::randomRange(width), math::RNG::randomRange(height)));

        while (!C.empty()) {
            // Select the element c using the selector.
            const auto idx = selector(C);
//            if (idx < 0 || idx >= C.size())
//                throw std::out_of_range("C has size " + std::to_string(C.size()) + ", selector tried to pick " + std::to_string(idx));
            const auto c = C[idx];
            const auto [x, y] = c;
            ci[x][y] = true;

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

        return Maze(getDimensions(), wi);
    }

    GrowingTreeMazeGenerator::Selector GrowingTreeMazeGenerator::getSelector(const CellSelectionStrategy css) {
        switch (css) {
            case CellSelectionStrategy::OLDEST: return [](const types::CellCollection &c) { return 0; };
            case CellSelectionStrategy::NEWEST: return [](const types::CellCollection &c) { return c.size() - 1; };
            case CellSelectionStrategy::MIDDLE: return [](const types::CellCollection &c) { return c.size() / 2; };
            case CellSelectionStrategy::RANDOM: return [](const types::CellCollection &c) { return math::RNG::randomRange(c.size()); };
        }
    }
}
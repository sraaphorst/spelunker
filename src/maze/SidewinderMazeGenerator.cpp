/**
 * SidewinderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/Direction.h>
#include <math/MathUtils.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "SidewinderMazeGenerator.h"

namespace spelunker::maze {
    SidewinderMazeGenerator::SidewinderMazeGenerator(const types::Dimensions2D &d, const double p)
        : MazeGenerator{d}, probabilityEast{p} {
        math::MathUtils::checkProbability(p);
    }

    SidewinderMazeGenerator::SidewinderMazeGenerator(int w, int h, double p)
        : SidewinderMazeGenerator{types::Dimensions2D{w, h}, p} {}

    SidewinderMazeGenerator::SidewinderMazeGenerator(const types::Dimensions2D &d)
            : SidewinderMazeGenerator{d, defaultProbabilityEast} {}

    SidewinderMazeGenerator::SidewinderMazeGenerator(int w, int h)
        : SidewinderMazeGenerator{types::Dimensions2D{w, h}, defaultProbabilityEast} {}

    const Maze SidewinderMazeGenerator::generate() const noexcept {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and then remove them iteratively.
        auto wi = createMazeLayout(getDimensions(), true);

        // Keep a collection of the cells visited so far.
        std::vector< types::Cell > cellRun;

        // Start in the top left, and continue down until we reach the bottom row.
        const auto maxX = width  - 1;
        const auto maxY = height - 1;

        for (auto y = 0; y < maxY; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Add this cell to the run.
                cellRun.emplace_back(types::cell(x, y));
                if (cellRun.size() > 1)
                    // We actually carve walls to the west for simplicity.
                    wi[rankPos(types::pos(x, y, types::Direction::WEST))] = false;

                // If we are at the end of the row or probability dictates we stop, add
                // a vertical cell, and empty out the run.
                if (x == maxX || math::RNG::randomProbability() > probabilityEast) {
                    const auto cell = math::RNG::randomElement(cellRun);
                    wi[rankPos(types::pos(cell, types::Direction::SOUTH))] = false;
                    cellRun.clear();
                }
            }
        }

        // The bottom row is all empty.
        for (auto x = 1; x < width; ++x)
            wi[rankPos(types::pos(x, maxY, types::Direction::WEST))] = false;

        return Maze(getDimensions(), wi);
    }
}
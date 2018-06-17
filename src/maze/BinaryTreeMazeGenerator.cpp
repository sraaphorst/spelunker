/**
 * BinaryTreeMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/Direction.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "BinaryTreeMazeGenerator.h"

namespace spelunker::maze {
    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(const types::Dimensions2D &d, const double p)
        : MazeGenerator{d}, eastProbability(p) {}

    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(int w, int h, double p)
        : BinaryTreeMazeGenerator{types::Dimensions2D{w, h}, p} {}

    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(const types::Dimensions2D &d)
        : BinaryTreeMazeGenerator{d, defaultEastProbability} {}

    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(int w, int h)
        : BinaryTreeMazeGenerator{types::Dimensions2D{w, h}, defaultEastProbability} {}

    const Maze BinaryTreeMazeGenerator::generate() const {
        const auto [width, height] = getDimensions();

        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                // Determine the directions (south / east) we can head from this cell.
                types::Direction d;
                if (x + 1 < width && y + 1 < height)
                    d = math::RNG::randomProbability() < eastProbability ? types::Direction::EAST : types::Direction::SOUTH;
                else if (x + 1 < width)
                    d = types::Direction::EAST;
                else if (y + 1 < height)
                    d = types::Direction::SOUTH;
                else
                    continue;

                const auto idx = rankPos(pos(x, y, d));
                wi[idx] = false;
            }

        return Maze(getDimensions(), wi);
    }
}
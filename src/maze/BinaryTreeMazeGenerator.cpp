/**
 * BinaryTreeMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include "types/CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "math/RNG.h"
#include "BinaryTreeMazeGenerator.h"

namespace spelunker::maze {
    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(int w, int h, double p)
            : MazeGenerator(w, h), eastProbability(p) {}

    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(int w, int h)
            : BinaryTreeMazeGenerator(w, h, 0.5) {}

    const Maze BinaryTreeMazeGenerator::generate() {
        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                // Determine the directions (south / east) we can head from this cell.
                types::Direction d;
                if (x + 1 < width && y + 1 < height)
                    d = math::RNG::randomProbability() < eastProbability ? types::EAST : types::SOUTH;
                else if (x + 1 < width)
                    d = types::EAST;
                else if (y + 1 < height)
                    d = types::SOUTH;
                else
                    continue;

                const auto idx = rankPos(pos(x, y, d));
                wi[idx] = false;
            }

        return Maze(width, height, wi);
    }
}
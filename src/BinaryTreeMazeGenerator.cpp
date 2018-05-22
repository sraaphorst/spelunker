/**
 * BinaryTreeMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "BinaryTreeMazeGenerator.h"

namespace vorpal::maze {
    BinaryTreeMazeGenerator::BinaryTreeMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze BinaryTreeMazeGenerator::generate() {
        // We start with all walls, and remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                // Determine the directions (south / east) we can head from this cell.
                std::vector<types::Direction> validDirs;
                if (x + 1 < width)  validDirs.emplace_back(types::Direction::EAST);
                if (y + 1 < height) validDirs.emplace_back(types::Direction::SOUTH);

                // If there are no directions, nothing to do.
                if (validDirs.empty())
                    continue;

                const auto d   = math::RNG::randomElement(validDirs);
                const auto idx = rankPos(types::pos(x, y, d));
                wi[idx] = false;
            }

        return Maze(width, height, wi);
    }
}
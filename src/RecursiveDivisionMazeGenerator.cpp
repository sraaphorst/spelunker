/**
 * RecursiveDivisionMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <queue>

#include "CommonMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "RecursiveDivisionMazeGenerator.h"

namespace spelunker::maze {
    RecursiveDivisionMazeGenerator::RecursiveDivisionMazeGenerator(int w, int h)
            : MazeGenerator(w, h) {}

    const Maze RecursiveDivisionMazeGenerator::generate() {
        // Unlike other algorithms, we start with no walls, and then add them iteratively.
        auto wi = initializeEmptyLayout(false);

        // Now create the container of rectangles to solve, and go through iteratively.
        std::queue<rectangle> areas;
        areas.push(rectangle(0, 0, width, height));

        while (!areas.empty()) {
            const auto area = areas.front();
            areas.pop();

            // Base case: if this area is an empty cell, ignore it.
            if (area.h == 1 && area.w == 1)
                continue;

            // First decide if we are splitting this area horizontally or vertically.
            // If the height is 1, we must split vertically.
            // Otherwise, if the width is 1, we must split horizontally.
            // Otherwise, we split in the direction that will result in the closest w:h ratio.
            const auto vertical = (area.h == 1) || (area.w > area.h);

            // Pick a valid cell in the range. If lowerBound = 0 and upperBound = 1,
            // then we get any cell in the range, but we want to omit the last row
            // or column since all walls will be E or S.
            const auto upperCell = vertical ? area.w : area.h;
            const auto p         = math::RNG::randomRange(upperCell-1);

            // Now we need a random gap in the wall.
            const auto gap = math::RNG::randomRange(vertical ? area.h : area.w);

            // Draw in the wall.
            const auto upperLoopBound = vertical ? area.h : area.w;
            for (int i=0; i < upperLoopBound; ++i) {
                if (i == gap)
                    continue;

                if (vertical)
                    wi[rankPos(types::pos(area.x + p, area.y + i, types::EAST))] = true;
                else
                    wi[rankPos(types::pos(area.x + i, area.y + p, types::SOUTH))] = true;
            }
            
            // Now split into two areas. Since we are always adding walls on the E and S,
            // we must do this carefully.
            if (vertical) {
                areas.push(rectangle(area.x, area.y, p + 1, area.h));
                areas.push(rectangle(area.x + p + 1, area.y, area.w - p - 1, area.h));
            } else {
                areas.push(rectangle(area.x, area.y, area.w, p + 1));
                areas.push(rectangle(area.x, area.y + p + 1, area.w, area.h - p - 1));
            }
        }

        return Maze(width, height, wi);
    }
}
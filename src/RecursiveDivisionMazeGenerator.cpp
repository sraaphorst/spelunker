/**
 * RecursiveDivisionMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <queue>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "RecursiveDivisionMazeGenerator.h"

namespace spelunker::maze {
    RecursiveDivisionMazeGenerator::RecursiveDivisionMazeGenerator(int w, int h,
                                                                   double lb, double ub,
                                                                   double p, bool alt)
            : MazeGenerator(w, h), lowerBound(lb), upperBound(ub), verticalProbability(p), alternate(alt) {}

    RecursiveDivisionMazeGenerator::RecursiveDivisionMazeGenerator(int w, int h)
            : RecursiveDivisionMazeGenerator(w, h, 0.0, 1.0, 0.5, false) {}

    const Maze RecursiveDivisionMazeGenerator::generate() {
        // Unlike other algorithms, we start with no walls, and then add them iteratively.
        auto wi = initializeEmptyLayout(false);

        // Now create the container of rectangles to solve, and go through iteratively.
        std::queue<rectangle> areas;
        areas.push(rectangle(0, 0, width, height));

        // We want to start by dividing up the greater dimension, so if width > height,
        // we want to divide width first: thus, set preVertical to what we don't wan't.
        auto prevVertical = width < height;

        while (!areas.empty()) {
            const auto area = areas.front();
            areas.pop();

            // Base case: if this area is an empty cell, ignore it.
            if (area.h == 1 && area.w == 1)
                continue;

            // First decide if we are splitting this area horizontally or vertically.
            // If the height is 1, we must split vertically.
            // Otherwise, if the width is 1, we must split horizontally.
            // Otherwise, we can split either way.
            const auto vertical = (area.h == 1)
                    || (area.w > 1
                        && ((alternate && !prevVertical) || (!alternate && math::RNG::randomProbability() < verticalProbability)));
            prevVertical = vertical;

            // Pick a valid cell in the range. If lowerBound = 0 and upperBound = 1,
            // then we get any cell in the range, but we want to omit the last row
            // or column since all walls will be E or S.
            const auto lowerCell = (int) (lowerBound * (vertical ? area.w : area.h));
            const auto upperCell = (int) (upperBound * (vertical ? area.w : area.h));
            const auto pos = (lowerCell >= upperCell) ? lowerCell : math::RNG::randomRange(lowerCell, std::min((vertical ? area.w : area.h) - 1, upperCell));

            // Now we need a random gap in the wall.
            const auto gap = math::RNG::randomRange(vertical ? area.h : area.w);

            // Draw in the wall.
            const auto upperLoopBound = vertical ? area.h : area.w;
            for (int i=0; i < upperLoopBound; ++i) {
                if (i == gap)
                    continue;

                if (vertical)
                    wi[rankPos(types::pos(area.x + pos, area.y + i, types::EAST))] = true;
                else
                    wi[rankPos(types::pos(area.x + i, area.y + pos, types::SOUTH))] = true;
            }

            // Now split into two areas. Since we are always adding walls on the E and S,
            // we must do this carefully.
            if (vertical) {
                areas.push(rectangle(area.x, area.y, pos + 1, area.h));
                areas.push(rectangle(area.x + pos + 1, area.y, area.w - pos - 1, area.h));
            } else {
                areas.push(rectangle(area.x, area.y, area.w, pos + 1));
                areas.push(rectangle(area.x, area.y + pos + 1, area.w, area.h - pos - 1));
            }
        }

        return Maze(width, height, wi);
    }
}
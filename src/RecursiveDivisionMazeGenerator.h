/**
 * RecursiveDivisionMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="http://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm"
 * recursive division algorithm.</a>
 */

#ifndef SPELUNKER_RECURSIVEDIVISIONMAZEGENERATOR_H
#define SPELUNKER_RECURSIVEDIVISIONMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;

    class RecursiveDivisionMazeGenerator final : public MazeGenerator {
    public:
        RecursiveDivisionMazeGenerator(int w, int h, double lb, double ub, double p, bool alt);
        RecursiveDivisionMazeGenerator(int w, int h);
        ~RecursiveDivisionMazeGenerator() = default;

        const Maze generate();
    private:
        /// A rectangle struct we use to represent sections of the maze to complete, to avoid recursion.
        struct rectangle {
            const int x, y, w, h;
            rectangle(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) {}
        };

        /// A lower bound on the sizes of the bisections.
        const double lowerBound;

        /// An upper bound on the sizes of the bisections.
        const double upperBound;

        /// The probability of choosing a vertical bisection versus a horizontal one.
        const double verticalProbability;

        /// A flag to force the algorithm to alternate between vertical and horizontal bisections. Overrides verticalProbability.
        const bool alternate;
    };
}

#endif //SPELUNKER_RECURSIVEDIVISIONMAZEGENERATOR_H

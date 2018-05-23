/**
 * SidewinderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the
 * <a href="http://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm">
 * sidewinder algorithm.</a>
 */

#ifndef SPELUNKER_SIDEWINDERMAZEGENERATOR_H
#define SPELUNKER_SIDEWINDERMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    class SidewinderMazeGenerator final : public MazeGenerator {
    public:
        SidewinderMazeGenerator(int w, int h, double p);
        SidewinderMazeGenerator(int w, int h);

        ~SidewinderMazeGenerator() = default;

        const Maze generate() override;

    private:
        const double probabilityEast;
    };
}

#endif //SPELUNKER_SIDEWINDERMAZEGENERATOR_H

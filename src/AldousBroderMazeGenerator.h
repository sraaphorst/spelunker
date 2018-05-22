/**
 * AldousBroderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="http://weblog.jamisbuck.org/2011/1/17/maze-generation-aldous-broder-algorithm">
 * Aldous-Broder algorithm.</a>
 *
 * Note this is a highly inefficient maze generation algorithm.
 */

#ifndef SPELUNKER_ALDOUSBRODERMAZEGENERATOR_H
#define SPELUNKER_ALDOUSBRODERMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;

    class AldousBroderMazeGenerator final : public MazeGenerator {
    public:
        AldousBroderMazeGenerator(int w, int h);
        virtual ~AldousBroderMazeGenerator() = default;

        const Maze generate() override;
    };
}

#endif //SPELUNKER_ALDOUSBRODERMAZEGENERATOR_H

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

    /// Maze generator using the Aldous-Broder approach to finding uniform spanning trees.
    /**
     * The Aldous-Broder approach to generating mazes is one of the slowest approaches, but typically
     * results in mazes without bias due to finding uniform spanning trees.
     * Note: uniform here means chosen at random.
     *
     * The algorithm proceeds as follows:
     *
     * Pick a random cell and marking it as visited.
     *
     * Until all cells are visited:
     * Pick a random neighbour and move to it.
     * If it it not visited, carve a wall between it and the previous cell.
     *
     * The reason that this algorithm is so ineffective is because there is no restriction to pick unvisited cells;
     * thus the carver meanders drunkenly about until it manages to have visited every cell, and in the process,
     * will visit many cells many times more than once.
     */
    class AldousBroderMazeGenerator final : public MazeGenerator {
    public:
        AldousBroderMazeGenerator(int w, int h);
        virtual ~AldousBroderMazeGenerator() = default;

        const Maze generate() override;
    };
}

#endif //SPELUNKER_ALDOUSBRODERMAZEGENERATOR_H

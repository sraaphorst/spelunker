/**
 * WilsonMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="http://weblog.jamisbuck.org/2011/1/20/maze-generation-wilson-s-algorithm.html">
 * Wilson algorithm.</a>
 */

#ifndef SPELUNKER_WILSONMAZEGENERATOR_H
#define SPELUNKER_WILSONMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /**
     * Wilson's algorithm generates mazes in the following way:
     *
     * Pick a random cell. Add it to the maze.
     * Continue until there are cells not in the maze:
     *     Pick a random cell not in the maze.
     *     Construct a random walk on the cell, maintaining the last direction
     *       taken in leaving the cell until you reach a cell in the maze.
     *     At that point, start back at the beginning cell, following the directions,
     *       removing walls and marking cells visited, until you arrive at a cell
     *       in the maze.
     *     Note that likely not every cell in the random walk will appear in the
     *       final path.
     */
    class WilsonMazeGenerator final : public MazeGenerator {
    public:
        WilsonMazeGenerator(int w, int h);
        ~WilsonMazeGenerator() final = default;

        const Maze generate() final;
    };
};

#endif //SPELUNKER_WILSONMAZEGENERATOR_H

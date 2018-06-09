/**
 * BFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using a randomized breadth-first search algorithm.
 */

#pragma once

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /**
     * A maze generator using a randomized breadth-first search.
     *
     * As far as I can tell, I haven't seen anything in the literature on this approach, probably because it will,
     * by nature of the breadth-first algorithm, result in mazes with very short paths from one corner to another.
     * Furthermore, the random point that was picked to seed the maze has a very obvious structure in the results,
     * as one would expect from a BFS algorithm.
     *
     * The idea here is that a random cell is chosen, marked as being visited, and all its neighbours are pushed to
     * the back of a queue.
     *
     * Until the queue is empty, the top cell is taken, and a list of its visited neighbours is found.
     * (There must be at least one visited neighbour by virtue of the cell being in the queue.)
     * A visited neighbour is chosen at random, and a passage is carved to it.
     *
     * The cell is then marked as visited, and its unvisited neighbours are pushed to the back of the queue.
     */
    class BFSMazeGenerator final : public MazeGenerator{
    public:
        BFSMazeGenerator(int w, int h);
        ~BFSMazeGenerator() final = default;

        const Maze generate() final;
    };
}

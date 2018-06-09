/**
 * RandomizedDFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Depth-first_search">
 * depth-first search algorithm.</a>
 */

#pragma once

#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /// Maze generator using the randomized depth-first search method.
    /**
     * This is a maze generator that uses the randomized depth-first search method.
     *
     * It is also called the backtracking method, because it begins at a point, and proceeds to carve paths as
     * long as possible until no longer able to do so. It then backtracks to the last visited cell with unvisited
     * neighbours, and begins carving passages at random to unvisited neighbours again.
     *
     * In order to avoid stack overflows, we simulate recursion using a stack.
     *
     * This results in mazes with very long, windy passages, wherein it is difficult to find the end position,
     * but easier to begin at the end and find the starting position.
     */
    class DFSMazeGenerator final : public MazeGenerator {
    public:
        DFSMazeGenerator(int w, int h);
        ~DFSMazeGenerator() final = default;

        const Maze generate() final;
    };
};


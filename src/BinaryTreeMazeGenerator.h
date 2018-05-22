/**
 * BinaryTreeMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the simplistic binary tree approach to generating a maze.
 */

#ifndef SPELUNKER_BINARYTREEMAZEGENERATOR_H
#define SPELUNKER_BINARYTREEMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace vorpal::maze {
    /// Maze generator using the binary tree approach.
    /**
     * The BinaryTreeMazeGenerator iterates over the maze, row-by-row (although visiting the cells in any order
     * would work), adding with 0.5 probability a wall to the east if possible, and 0.5 probability a wall to the
     * south if possible. In some cases (e.g. the rightmost column), we will not be able to add east walls, and
     * in one case (i.e. the bottom rightmost cell), we will not be able to add anything.
     */
    class BinaryTreeMazeGenerator final : public MazeGenerator {
    public:
        BinaryTreeMazeGenerator(int w, int h);
        virtual ~BinaryTreeMazeGenerator() = default;

        const Maze generate() override;
    };
}

#endif //SPELUNKER_BINARYTREEMAZEGENERATOR_H

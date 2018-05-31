/**
 * RandomizedPrimMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
  * A maze generator using the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Prim's_algorithm">
 * Randomized Prim's algorithm.</a>
 */

#ifndef SPELUNKER_RANDOMIZEDPRIMMAZEGENERATOR_H
#define SPELUNKER_RANDOMIZEDPRIMMAZEGENERATOR_H

#include "CommonMazeAttributes.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /// A @see{MazeGenerator} using the randomized Prim method.
    class PrimMazeGenerator final : public MazeGenerator {
    public:
        PrimMazeGenerator(int w, int h);
        ~PrimMazeGenerator() final = default;

        const Maze generate() final;

    private:
        /// Add the non-exterior walls of a cell to the wall list, with one possible omission.
        void addCellWalls(const types::Cell &c, WallCollection &wallList, const WallIncidence &wi);
    };
};

#endif //SPELUNKER_RANDOMIZEDPRIMMAZEGENERATOR_H

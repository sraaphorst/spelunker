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

#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;

    /// A @see{MazeGenerator} using the randomized Prim method.
    class RandomizedPrimMazeGenerator final : public MazeGenerator {
    public:
        RandomizedPrimMazeGenerator(int w, int h);
        virtual ~RandomizedPrimMazeGenerator() = default;

        const Maze generate() override;

    private:
        using CellRowIndicator = std::vector<bool>;
        using CellIndicator = std::vector<CellRowIndicator>;
        using WallList = std::vector<types::WallID>;

        /// Add the non-exterior walls of a cell to the wall list, with one possible omission.
        void addCellWalls(const types::Cell &c, WallList &wallList, const types::WallIncidence &wi);
    };
};

#endif //SPELUNKER_RANDOMIZEDPRIMMAZEGENERATOR_H

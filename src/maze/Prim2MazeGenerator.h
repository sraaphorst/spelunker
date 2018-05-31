/**
 * RandomizedPrim2MazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This is a modification of Prim's algorithm that results in more branching.
 * See here: <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Modified_version">
 * Modified Prim's algorithm.</a>
 */

#ifndef SPELUNKER_RANDOMIZEDPRIM2MAZEGENERATOR_H
#define SPELUNKER_RANDOMIZEDPRIM2MAZEGENERATOR_H

#include "types/CommonMazeAttributes.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    class Prim2MazeGenerator final : public MazeGenerator {
    public:
        Prim2MazeGenerator(int w, int h);
        virtual ~Prim2MazeGenerator() = default;

        const Maze generate() override;

    private:
        /// Add the unvisited neighbours of a cell to the list to process.
        void addUnivisitedNeighbourCells(const types::Cell &c,
                                         types::CellCollection &cells,
                                         const types::CellIndicator &ci);
    };
};

#endif //SPELUNKER_RANDOMIZEDPRIM2MAZEGENERATOR_H

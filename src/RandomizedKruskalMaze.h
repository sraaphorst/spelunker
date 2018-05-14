/**
 * RandomizedKruskalMaze.h
 *
 * A maze that is generated via the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Kruskal's_algorithm">
 * Randomized Kruskal's algorithm.</a>
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_RANDOMIZEDKRUSKALMAZE_H
#define SPELUNKER_RANDOMIZEDKRUSKALMAZE_H

#include "maze.h"

namespace vorpal::maze {
    class RandomizedKruskalMaze final : public Maze {
    public:
        RandomizedKruskalMaze(const int w,
                              const int h,
                              const Cell &s,
                              const CellCollection &ends);

        const WallIncidence generate() override;
    };
}
#endif //SPELUNKER_RANDOMIZEDKRUSKALMAZE_H

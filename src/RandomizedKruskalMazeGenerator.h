/**
 * RandomizedKruskalMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Kruskal's_algorithm">
 * Randomized Kruskal's algorithm.</a>
 */

#ifndef SPELUNKER_RANDOMIZEDKRUSKALMAZE_H
#define SPELUNKER_RANDOMIZEDKRUSKALMAZE_H

#include <vector>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace vorpal::maze {
    class RandomizedKruskalMazeGenerator final : public MazeGenerator {
    public:
        RandomizedKruskalMazeGenerator(int w, int h);
        virtual ~RandomizedKruskalMazeGenerator() = default;

        const Maze generate() override;
    };
}
#endif //SPELUNKER_RANDOMIZEDKRUSKALMAZE_H

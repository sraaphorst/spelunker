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

#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;

    /// A @see{MazeGenerator} using the randomized Kruskal method.
    class KruskalMazeGenerator final : public MazeGenerator {
    public:
        KruskalMazeGenerator(int w, int h);
        virtual ~KruskalMazeGenerator() = default;

        const Maze generate() override;
    };
}
#endif //SPELUNKER_RANDOMIZEDKRUSKALMAZE_H

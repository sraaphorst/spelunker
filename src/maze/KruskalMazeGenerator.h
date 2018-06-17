/**
 * RandomizedKruskalMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Kruskal's_algorithm">
 * Randomized Kruskal's algorithm.</a>
 */

#pragma once

#include <vector>

#include <types/Dimensions2D.h>

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /// A @see{MazeGenerator} using the randomized Kruskal method.
    class KruskalMazeGenerator final : public MazeGenerator {
    public:
        KruskalMazeGenerator(const types::Dimensions2D &d);
        KruskalMazeGenerator(int w, int h);
        ~KruskalMazeGenerator() final = default;

        const Maze generate() const noexcept final;
    };
}


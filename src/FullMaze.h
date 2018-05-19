/**
 * FullMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_FULLMAZE_H
#define SPELUNKER_FULLMAZE_H

#include "MazeGenerator.h"

namespace vorpal::maze {
    class FullMaze : public MazeGenerator {
    public:
        FullMaze(int w, int h) : MazeGenerator(w, h) {}

        const Maze generate() override {
            auto wi = initializeEmptyLayout(false);
            return Maze(width, height, wi);
        }
    };
};


#endif //SPELUNKER_FULLMAZE_H

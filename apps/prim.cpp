/**
 * prim.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized Prim algorithm.
 */

#include "Executor.h"
#include "PrimMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::PrimMazeGenerator>::generateAndDisplayMaze(argc, argv);
}

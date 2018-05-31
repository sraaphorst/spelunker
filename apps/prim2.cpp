/**
 * prim2.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the modified randomized Prim algorithm.
 */

#include "Executor.h"
#include "maze/Prim2MazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::Prim2MazeGenerator>::generateAndDisplayMaze(argc, argv);
}

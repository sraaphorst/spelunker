/**
 * prim.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized Prim algorithm.
 */

#include "Executor.h"
#include "RandomizedPrimMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::RandomizedPrimMazeGenerator>::generateAndDisplayMaze(argc, argv);
}

/**
 * dfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized DFS algorithm.
 */

#include "RandomizedDFSMazeGenerator.h"
#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::RandomizedDFSMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
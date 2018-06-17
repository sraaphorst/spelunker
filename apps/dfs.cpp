/**
 * dfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized DFS algorithm.
 */

#include <maze/DFSMazeGenerator.h>

#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::DFSMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
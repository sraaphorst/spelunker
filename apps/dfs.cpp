/**
 * dfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized DFS algorithm.
 */

#include "DFSMazeGenerator.h"
#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::DFSMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
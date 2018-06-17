/**
 * bfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized BFS algorithm.
 */

#include <maze/BFSMazeGenerator.h>

#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::BFSMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
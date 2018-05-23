/**
 * bfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized BFS algorithm.
 */

#include "Executor.h"
#include "BFSMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::BFSMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
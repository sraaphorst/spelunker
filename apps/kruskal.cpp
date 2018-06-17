/**
 * kruskal.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized Kruskal algorithm.
 */

#include <maze/KruskalMazeGenerator.h>

#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::KruskalMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
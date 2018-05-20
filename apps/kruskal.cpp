/**
 * kruskal.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the randomized Kruskal algorithm.
 */

#include "Executor.h"
#include "RandomizedKruskalMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::RandomizedKruskalMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
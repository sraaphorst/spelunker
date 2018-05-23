/**
 * wilson.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the Wilson algorithm.
 */

#include "Executor.h"
#include "WilsonMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::WilsonMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
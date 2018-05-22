/**
 * wilson.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the Wilson algorithm.
 */

#include "WilsonMazeGenerator.h"
#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::WilsonMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
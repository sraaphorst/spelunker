/**
 * recursive_division.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the recursive division algorithm, which is highly configurable, to create a maze.
 */

#include <iostream>

#include "Executor.h"
#include "Maze.h"
#include "Show.h"
#include "RecursiveDivisionMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::RecursiveDivisionMazeGenerator>::generateAndDisplayMaze(argc, argv);
}

/**
 * recursive_division.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the recursive division algorithm, which is highly configurable, to create a maze.
 */

#include <iostream>

#include <typeclasses/Show.h>
#include <maze/Maze.h>
#include <maze/RecursiveDivisionMazeGenerator.h>

#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::RecursiveDivisionMazeGenerator>::generateAndDisplayMaze(argc, argv);
}

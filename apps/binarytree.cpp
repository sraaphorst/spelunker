/**
 * binarytree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the binary tree algorithm.
 */

#include "Executor.h"
#include "BinaryTreeMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<vorpal::maze::BinaryTreeMazeGenerator>::generateAndDisplayMaze(argc, argv);
}

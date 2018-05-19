/**
 * mazetest.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "Maze.h"
#include "RandomizedKruskalMazeGenerator.h"
#include "FullMaze.h"

using namespace std;
using namespace vorpal::maze;
using namespace vorpal::typeclasses;

int main(int argc, char *argv[]) {
    RandomizedKruskalMazeGenerator g(40, 30);
    const Maze m = g.generate();
    cout << Show<Maze>::show(m) << endl;
    return 0;
}
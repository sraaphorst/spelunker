/**
 * cellular_automaton.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a ThickMaze via the CellularAutomatonThickMazeGenerator.
 */

#include <iostream>

#include "CellularAutomatonThickMazeGenerator.h"
#include "Show.h"
#include "ThickMaze.h"
using namespace spelunker::thickmaze;

int main(int argc, char *argv[]) {
    CellularAutomatonThickMazeGenerator::settings s;
    CellularAutomatonThickMazeGenerator gen(80, 80, s);
    ThickMaze tm = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
}
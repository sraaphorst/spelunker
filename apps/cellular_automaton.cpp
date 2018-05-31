/**
 * cellular_automaton.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a ThickMaze via the CellularAutomatonThickMazeGenerator.
 */

#include <iostream>

#include "thickmaze/CellularAutomatonThickMazeGenerator.h"
#include "typeclasses/Show.h"
#include "thickmaze/ThickMaze.h"
using namespace spelunker::thickmaze;

int main(int argc, char *argv[]) {
    CellularAutomatonThickMazeGenerator::settings s;
    CellularAutomatonThickMazeGenerator gen(50, 40, s);
    ThickMaze tm = gen.generate();//.reverse();
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
}
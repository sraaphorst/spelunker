/**
 * cellular_automaton.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a ThickMaze via the CellularAutomatonThickMazeGenerator.
 */

#include <iostream>

#include <typeclasses/Show.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>

using namespace spelunker::thickmaze;

int main(int argc, char *argv[]) {
    CellularAutomatonThickMazeGenerator::settings s{};
    CellularAutomatonThickMazeGenerator gen{100, 50, s};
    ThickMaze tm = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
    return 0;
}
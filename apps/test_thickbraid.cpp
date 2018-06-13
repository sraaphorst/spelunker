/**
 * test_thickbraid.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "maze/DFSMazeGenerator.h"
#include "maze/Maze.h"
#include "maze/MazeTypeclasses.h"
#include "thickmaze/ThickMaze.h"
#include "thickmaze/ThickMazeTypeclasses.h"
#include "typeclasses/Homomorphism.h"
#include "typeclasses/Show.h"

using namespace spelunker;

int main(int argc, char *argv[]) {
    maze::DFSMazeGenerator gen(22, 16);
    maze::Maze m = gen.generate();
    std::cout << typeclasses::Show<maze::Maze>::show(m);

    thickmaze::ThickMaze tm = typeclasses::Homomorphism<maze::Maze, thickmaze::ThickMaze>::morph(m);
    std::cout << typeclasses::Show<thickmaze::ThickMaze>::show(tm);

    spelunker::thickmaze::ThickMaze btm = tm.braid();
    std::cout << typeclasses::Show<thickmaze::ThickMaze>::show(btm);
    return 0;
}
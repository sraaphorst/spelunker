/**
 * grid_colouring.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "thickmaze/GridColouring.h"
#include "thickmaze/GridColouringThickMazeGenerator.h"
#include "thickmaze/ThickMaze.h"
#include "typeclasses/Show.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " ux vx vy" << std::endl;
        return 1;
    }

    auto ux = Utils::parseLong(argv[1]);
    if (ux <= 0) {
        std::cout << "Illegal value for ux: " << argv[1] << std::endl;
        return 2;
    }

    auto vx = Utils::parseLong(argv[2]);
    if (vx < 0) {
        std::cout << "Illegal value for vx: " << argv[2] << std::endl;
    }

    auto vy = Utils::parseLong(argv[3]);
    if (vy <= 0) {
        std::cout << "Illegal value for vy: " << argv[3] << std::endl;
    }

    //spelunker::thickmaze::GridColouring gc(5, 3, 1);
    spelunker::thickmaze::GridColouring gc(ux, vx, vy);
    auto candidates = gc.wallCandidates();
    for (auto i=0; i < candidates.size(); ++i) {
        std::cout << "************* CANDIDATE " << i << " **********" << std:: endl;
        auto candidate = gc.wallCandidates()[i];
        spelunker::thickmaze::GridColouringThickMazeGenerator gen(50, 40, gc, candidate);
        spelunker::thickmaze::ThickMaze tm = gen.generate();
        std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
        std::cout << std::endl;
    }
    return 0;
}
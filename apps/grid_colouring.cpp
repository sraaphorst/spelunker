/**
 * grid_colouring.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "thickmaze/GridColouring.h"
#include "thickmaze/GridColouringThickMazeGenerator.h"
#include "thickmaze/ThickMaze.h"
#include "thickmaze/ThickMazeTypeclasses.h"
#include "typeclasses/Show.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cout << "Usage: " << argv[0] << " width height ux vx vy" << std::endl;
        return 1;
    }

    auto width = Utils::parseLong(argv[1]);
    if (width <= 0) {
        std::cerr << "Illegal value for width: " << argv[1] << std::endl;
        return 2;
    }

    auto height = Utils::parseLong(argv[2]);
    if (height <= 0) {
        std::cerr << "Illegal value for height: " << argv[2] << std::endl;
        return 3;
    }

    auto ux = Utils::parseLong(argv[3]);
    if (ux <= 0) {
        std::cerr << "Illegal value for ux: " << argv[3] << std::endl;
        return 4;
    }

    auto vx = Utils::parseLong(argv[4]);
    if (vx < 0) {
        std::cerr << "Illegal value for vx: " << argv[4] << std::endl;
        return 5;
    }

    auto vy = Utils::parseLong(argv[5]);
    if (vy <= 0) {
        std::cerr << "Illegal value for vy: " << argv[5] << std::endl;
        return 6;
    }

    spelunker::thickmaze::GridColouring gc(ux, vx, vy);
    if (width < gc.numCols()) {
        std::cerr << "For the parameters supplied, width must be at least " << gc.numCols() << '.' << std::endl;
        return 7;
    }
    if (height < gc.numRows()) {
        std::cerr << "For the parameters supplied, height must be at least " << gc.numRows() << '.' << std::endl;
        return 8;
    }

    auto candidates = gc.wallCandidates();
    for (auto i=0; i < candidates.size(); ++i) {
        auto candidate = candidates[i];
        std::cout << "************* CANDIDATE " << i << " (" <<
                  spelunker::typeclasses::Show<spelunker::thickmaze::GridColouring::CandidateConfiguration>::show(candidate) <<
                  ") **********" << std:: endl;
        spelunker::thickmaze::GridColouringThickMazeGenerator gen(width, height, gc, candidate);
        spelunker::thickmaze::ThickMaze tm = gen.generate();
        std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
        std::cout << std::endl;
    }
    return 0;
}
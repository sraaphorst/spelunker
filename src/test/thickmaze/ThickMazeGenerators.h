/**
 * ThickMazeGenerators.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

#include <types/Dimensions2D.h>
#include <maze/MazeGenerator.h>
#include <maze/AldousBroderMazeGenerator.h>
#include <maze/BFSMazeGenerator.h>
#include <maze/BinaryTreeMazeGenerator.h>
#include <maze/DFSMazeGenerator.h>
#include <maze/EllerMazeGenerator.h>
#include <maze/GrowingTreeMazeGenerator.h>
#include <maze/HuntAndKillMazeGenerator.h>
#include <maze/KruskalMazeGenerator.h>
#include <maze/PrimMazeGenerator.h>
#include <maze/Prim2MazeGenerator.h>
#include <maze/RecursiveDivisionMazeGenerator.h>
#include <maze/SidewinderMazeGenerator.h>
#include <maze/WilsonMazeGenerator.h>
#include <thickmaze/ThickMazeGenerator.h>
#include <thickmaze/ThickMazeGeneratorByHomomorphism.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/GridColouring.h>
#include <thickmaze/GridColouringThickMazeGenerator.h>
#include <math/RNG.h>
using namespace spelunker;

// Return a list of all TihickMazeGenerators.
using ThickMazeGenerators = std::vector<thickmaze::ThickMazeGenerator*>;

const ThickMazeGenerators createThickMazeGenerators(const types::Dimensions2D &d) {
    const types::Dimensions2D dhalf = d/2;
    const thickmaze::GridColouring gridColouring{4, 1, 2};
    const thickmaze::GridColouring::CandidateConfigurationCollection cfgColl = gridColouring.wallCandidates(10);
    const thickmaze::GridColouring::CandidateConfiguration cfg = math::RNG::randomElement(cfgColl);

    return ThickMazeGenerators {
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::AldousBroderMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::BFSMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::BinaryTreeMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::DFSMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::EllerMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::GrowingTreeMazeGenerator{dhalf, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::HuntAndKillMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::KruskalMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::PrimMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::Prim2MazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::RecursiveDivisionMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::SidewinderMazeGenerator{dhalf}},
        new thickmaze::ThickMazeGeneratorByHomomorphism{new maze::WilsonMazeGenerator{dhalf}},
        new thickmaze::CellularAutomatonThickMazeGenerator{d, thickmaze::CellularAutomatonThickMazeGenerator::settings{}},
        new thickmaze::GridColouringThickMazeGenerator{d, gridColouring, cfg}
    };
}

void deleteThickMazeGenerators(ThickMazeGenerators &mgs) {
    while (!mgs.empty()) {
        auto mg = mgs.back();
        mgs.pop_back();
        delete mg;
    }
}
/**
 * MazeGenerators.h
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
using namespace spelunker;

// Return a list of all MazeGenerators.
using MazeGenerators = std::vector<maze::MazeGenerator*>;

MazeGenerators createGenerators(const types::Dimensions2D &d) {
    return std::vector<maze::MazeGenerator*> {
        new maze::AldousBroderMazeGenerator{d},
        new maze::BFSMazeGenerator{d},
        new maze::BinaryTreeMazeGenerator{d},
        new maze::EllerMazeGenerator{d},
        new maze::GrowingTreeMazeGenerator{d, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM},
        new maze::HuntAndKillMazeGenerator{d},
        new maze::KruskalMazeGenerator{d},
        new maze::PrimMazeGenerator{d},
        new maze::Prim2MazeGenerator{d},
        new maze::RecursiveDivisionMazeGenerator{d},
        new maze::SidewinderMazeGenerator{d},
        new maze::WilsonMazeGenerator{d}
    };
}

void deleteGenerators(MazeGenerators &mgs) {
    while (!mgs.empty()) {
        auto mg = mgs.back();
        mgs.pop_back();
        delete mg;
    }
}

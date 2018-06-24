/**
 * MazeGenerators.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
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
namespace spelunker::maze {
    /**
     * This is just a quick and dirty class of all MazeGenerators, used
     * strictly for testing purposes.
     */
    class MazeGenerators {
    public:
        using MazeGeneratorCollection = std::vector<std::unique_ptr<maze::MazeGenerator>>;
        static auto constexpr width  = 50;
        static auto constexpr height = 40;

        explicit MazeGenerators(const types::Dimensions2D &d = types::Dimensions2D{width, height})
            : dim{d} {

            // I want to use std::make_unique here, but no matter what I do, Travis fails.
            gens.emplace_back(std::unique_ptr<maze::AldousBroderMazeGenerator>(new maze::AldousBroderMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::BFSMazeGenerator>(new maze::BFSMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::BinaryTreeMazeGenerator>(new maze::BinaryTreeMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::DFSMazeGenerator>(new maze::DFSMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::EllerMazeGenerator>(new maze::EllerMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::GrowingTreeMazeGenerator>(new maze::GrowingTreeMazeGenerator{d, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM}));
            gens.emplace_back(std::unique_ptr<maze::HuntAndKillMazeGenerator>(new maze::HuntAndKillMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::KruskalMazeGenerator>(new maze::KruskalMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::PrimMazeGenerator>(new maze::PrimMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::Prim2MazeGenerator>(new maze::Prim2MazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::RecursiveDivisionMazeGenerator>(new maze::RecursiveDivisionMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::SidewinderMazeGenerator>(new maze::SidewinderMazeGenerator{d}));
            gens.emplace_back(std::unique_ptr<maze::WilsonMazeGenerator>(new maze::WilsonMazeGenerator{d}));
        }

        inline const MazeGeneratorCollection &getGenerators() const {
            return gens;
        }

    private:
        const types::Dimensions2D dim;
        MazeGeneratorCollection gens;
    };
}

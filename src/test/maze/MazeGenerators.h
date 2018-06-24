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

            gens.emplace_back(std::make_unique<maze::AldousBroderMazeGenerator>(maze::AldousBroderMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::BFSMazeGenerator>(maze::BFSMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::BinaryTreeMazeGenerator>(maze::BinaryTreeMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::DFSMazeGenerator>(maze::DFSMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::EllerMazeGenerator>(maze::EllerMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::GrowingTreeMazeGenerator>(maze::GrowingTreeMazeGenerator{d, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM}));
            gens.emplace_back(std::make_unique<maze::HuntAndKillMazeGenerator>(maze::HuntAndKillMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::KruskalMazeGenerator>(maze::KruskalMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::PrimMazeGenerator>(maze::PrimMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::Prim2MazeGenerator>(maze::Prim2MazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::RecursiveDivisionMazeGenerator>(maze::RecursiveDivisionMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::SidewinderMazeGenerator>(maze::SidewinderMazeGenerator{d}));
            gens.emplace_back(std::make_unique<maze::WilsonMazeGenerator>(maze::WilsonMazeGenerator{d}));
        }

        inline const MazeGeneratorCollection &getGenerators() const {
            return gens;
        }

    private:
        const types::Dimensions2D dim;
        MazeGeneratorCollection gens;
    };
}

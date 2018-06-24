/**
 * ThickMazeGenerators.h
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
#include <thickmaze/ThickMazeGenerator.h>
#include <thickmaze/ThickMazeGeneratorByHomomorphism.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/GridColouring.h>
#include <thickmaze/GridColouringThickMazeGenerator.h>
#include <math/RNG.h>

namespace spelunker::thickmaze {
    /**
     * This is just a quick and dirty class of all ThickMazeGenerators, used
     * strictly for testing purposes.
     */
    class ThickMazeGenerators {
    public:
        using ThickMazeGeneratorCollection = std::vector<std::unique_ptr<thickmaze::ThickMazeGenerator>>;
        static auto constexpr width  = 50;
        static auto constexpr height = 40;

        explicit ThickMazeGenerators(const types::Dimensions2D &d = types::Dimensions2D{width, height})
            : dim{d} {

            const auto dhalf = d / 2;

            // I want to use std::make_unique here, but no matter what I do, Travis fails.
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::AldousBroderMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::AldousBroderMazeGenerator>(maze::AldousBroderMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::BFSMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::BFSMazeGenerator>(maze::BFSMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::BinaryTreeMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::BinaryTreeMazeGenerator>(maze::BinaryTreeMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::DFSMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::DFSMazeGenerator>(maze::DFSMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::EllerMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::EllerMazeGenerator>(maze::EllerMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::GrowingTreeMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::GrowingTreeMazeGenerator>(maze::GrowingTreeMazeGenerator{dhalf, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::HuntAndKillMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::HuntAndKillMazeGenerator>(maze::HuntAndKillMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::KruskalMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::KruskalMazeGenerator>(maze::KruskalMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::PrimMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::PrimMazeGenerator>(maze::PrimMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::Prim2MazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::Prim2MazeGenerator>(maze::Prim2MazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::RecursiveDivisionMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::RecursiveDivisionMazeGenerator>(maze::RecursiveDivisionMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::SidewinderMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::SidewinderMazeGenerator>(maze::SidewinderMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::ThickMazeGeneratorByHomomorphism<maze::WilsonMazeGenerator>>(new thickmaze::ThickMazeGeneratorByHomomorphism<maze::WilsonMazeGenerator>(maze::WilsonMazeGenerator{dhalf})));
            gens.emplace_back(std::unique_ptr<thickmaze::CellularAutomatonThickMazeGenerator>(new thickmaze::CellularAutomatonThickMazeGenerator{dim, caSettings}));

            const thickmaze::GridColouring::CandidateConfigurationCollection cfgColl = gridColouring.wallCandidates(10);
            const thickmaze::GridColouring::CandidateConfiguration cfg = math::RNG::randomElement(cfgColl);
            gens.emplace_back(std::unique_ptr<thickmaze::GridColouringThickMazeGenerator>(new thickmaze::GridColouringThickMazeGenerator(dim, gridColouring, cfg)));
        }

        inline const ThickMazeGeneratorCollection &getGenerators() const {
            return gens;
        }

    private:
        const types::Dimensions2D dim;
        const thickmaze::GridColouring gridColouring{4, 1, 2};
        const thickmaze::CellularAutomatonThickMazeGenerator::settings caSettings{};

        ThickMazeGeneratorCollection gens;
    };
}

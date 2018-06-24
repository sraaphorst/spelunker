/**
 * ThickMazeGenerators.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <Travis.h>
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

            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::AldousBroderMazeGenerator>>(maze::AldousBroderMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::BFSMazeGenerator>>(maze::BFSMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::BinaryTreeMazeGenerator>>(maze::BinaryTreeMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::DFSMazeGenerator>>(maze::DFSMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::EllerMazeGenerator>>(maze::EllerMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::GrowingTreeMazeGenerator>>(maze::GrowingTreeMazeGenerator{dhalf, maze::GrowingTreeMazeGenerator::CellSelectionStrategy::RANDOM}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::HuntAndKillMazeGenerator>>(maze::HuntAndKillMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::KruskalMazeGenerator>>(maze::KruskalMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::PrimMazeGenerator>>(maze::PrimMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::Prim2MazeGenerator>>(maze::Prim2MazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::RecursiveDivisionMazeGenerator>>(maze::RecursiveDivisionMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::SidewinderMazeGenerator>>(maze::SidewinderMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::ThickMazeGeneratorByHomomorphism<maze::WilsonMazeGenerator>>(maze::WilsonMazeGenerator{dhalf}));
            gens.emplace_back(std::make_unique<thickmaze::CellularAutomatonThickMazeGenerator>(thickmaze::CellularAutomatonThickMazeGenerator{dim, caSettings}));

            const thickmaze::GridColouring::CandidateConfigurationCollection cfgColl = gridColouring.wallCandidates(10);
            const thickmaze::GridColouring::CandidateConfiguration cfg = math::RNG::randomElement(cfgColl);
            gens.emplace_back(std::make_unique<thickmaze::GridColouringThickMazeGenerator>(dim, gridColouring, cfg));
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

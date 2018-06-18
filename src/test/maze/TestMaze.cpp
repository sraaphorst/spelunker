#include <catch.hpp>

#include <string>

#include <types/Dimensions2D.h>
#include <types/Symmetry.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>
#include "MazeGenerators.h"

using namespace spelunker;

TEST_CASE("Rectangular mazes can be manipulated via certain symmetries", "[maze][symmetry]") {
    int constexpr width = 50;
    int constexpr height = 40;

    auto gens = createGenerators(types::Dimensions2D{width, height});

    SECTION("Creating mazes maintains the right size") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            REQUIRE(m.getWidth() == width);
            REQUIRE(m.getHeight() == height);
        }
    }
//    SECTION("Rotating twice by 90 degrees is the same as rotating by 180 degrees") {
//        for (const auto gen: gens) {
//            const maze::Maze m = gen->generate();
//            const maze::Maze m9090 = m
//                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
//                    .applySymmetry(types::Symmetry::ROTATION_BY_90);
//            const maze::Maze m180 = m.applySymmetry(types::Symmetry::ROTATION_BY_180);
//
//            REQUIRE(m9090 == m180);
//        }
//    }

    deleteGenerators(gens);
}

/**
 * TestMazeSymmetries.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <string>

#include <types/Dimensions2D.h>
#include <types/Transformation.h>
#include <typeclasses/Show.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>
#include "MazeGenerators.h"

using namespace spelunker;

TEST_CASE("Rectangular Mazes can be manipulated via certain transformations", "[maze][symmetry][rectangle]") {
    constexpr auto width  = 50;
    constexpr auto height = 40;
    const types::Dimensions2D &dim{ width, height };

    // Get all the generators and all the transformations.
    const auto mg    = maze::MazeGenerators{dim};
    const auto &gens = mg.getGenerators();
    const auto syms  = types::transformations();

    SECTION("All generators produce mazes of the correct size: " + typeclasses::Show<types::Dimensions2D>::show(dim)) {
        for (const auto &gen: gens) {
            const auto m = gen->generate();
            REQUIRE(m.getWidth() == width);
            REQUIRE(m.getHeight() == height);
        }
    }

    SECTION("A Maze under the action of two transformations equals the Maze under the action of their composition") {
        for (const auto s1: syms) {
            if (s1 == types::Transformation::REFLECTION_IN_NESW || s1 == types::Transformation::REFLECTION_IN_NWSE)
                continue;
            for (const auto s2: syms) {
                if (s2 == types::Transformation::REFLECTION_IN_NESW || s2 == types::Transformation::REFLECTION_IN_NWSE)
                    continue;
                const auto sc = types::composeTransformations(s1, s2);
                if (sc == types::Transformation::REFLECTION_IN_NESW || sc == types::Transformation::REFLECTION_IN_NWSE)
                    continue;

                for (const auto &gen: gens) {
                    const auto m = gen->generate();
                    const auto ms = m.applyTransformation(s1).applyTransformation(s2);
                    const auto mc = m.applyTransformation(sc);
                    REQUIRE(ms == mc);
                }
            }
        }
    }

    SECTION("Attempts to operate on a rectangular Maze with diagonal transformations should cause an exception") {
        for (const auto s1: syms) {
            for (const auto s2: syms) {
                const auto sc = types::composeTransformations(s1, s2);
                if (s1 != types::Transformation::REFLECTION_IN_NESW && s1 != types::Transformation::REFLECTION_IN_NWSE &&
                    s2 != types::Transformation::REFLECTION_IN_NESW && s2 != types::Transformation::REFLECTION_IN_NWSE &&
                    sc != types::Transformation::REFLECTION_IN_NESW && sc != types::Transformation::REFLECTION_IN_NWSE)
                    continue;
                for (const auto &gen: gens) {
                    const auto m = gen->generate();
                    REQUIRE_THROWS(m.applyTransformation(s1).applyTransformation(s2) == m.applyTransformation(sc));
                }
            }
        }
    }
}

TEST_CASE("Square Mazes can be manipulated via all transformations", "[maze][symmetry][square]") {
    constexpr auto side = 50;
    const auto dim = types::Dimensions2D{side, side};

    // Get all the generators and all the transformations.
    const auto mg    = maze::MazeGenerators{dim};
    const auto &gens = mg.getGenerators();
    const auto syms = types::transformations();

    SECTION("All generators produce Mazes of the correct size: " + typeclasses::Show<types::Dimensions2D>::show(dim)) {
        for (const auto &gen: gens) {
            const auto m = gen->generate();
            REQUIRE(m.getWidth() == side);
            REQUIRE(m.getHeight() == side);
        }
    }

    SECTION("A Maze under the action of two transformations equals the Maze under the action of their composition") {
        for (const auto s1: syms) {
            for (const auto s2: syms) {
                const auto sc = types::composeTransformations(s1, s2);

                for (const auto &gen: gens) {
                    const auto m = gen->generate();
                    const auto ms = m.applyTransformation(s1).applyTransformation(s2);
                    const auto mc = m.applyTransformation(sc);
                    REQUIRE(ms == mc);
                }
            }
        }
    }
}

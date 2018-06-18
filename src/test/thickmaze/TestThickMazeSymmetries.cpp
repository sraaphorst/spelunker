/**
 * TestThickMazeSymmetries.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <string>

#include <types/Dimensions2D.h>
#include <types/Symmetry.h>
#include <typeclasses/Show.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeGenerator.h>
#include "ThickMazeGenerators.h"

using namespace spelunker;


TEST_CASE("Rectangular ThickMazes can be manipulated via certain symmetries", "[thickmaze][symmetry][rectangle]") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const auto dim = types::Dimensions2D{width, height};

    // ThickMaze constructions from regular mazes produce size 2w-1 by 2h-1.
    const auto dim_1 = types::Dimensions2D{width-1, height-1};

    // Get all the generators and all the symmetries.
    auto gens = createThickMazeGenerators(dim);
    const auto syms = types::symmetries();

    SECTION("All generators produce ThickMazes of the correct size: " +
            typeclasses::Show<types::Dimensions2D>::show(dim) + " or " +
            typeclasses::Show<types::Dimensions2D>::show(dim_1)) {
        for (const auto gen: gens) {
            const auto m = gen->generate();

            // Use approx to allow width / height or width - 1 / height - 1 (for the hommorphism constructions).
            REQUIRE(m.getWidth() == Approx(width - 0.5).epsilon(0.5));
            REQUIRE(m.getHeight() == Approx(height - 0.5).epsilon(0.5));
        }
    }

    SECTION("A ThickMaze under the action of two symmetries equals the ThickMaze under the action of their composition") {
        for (const auto s1: syms) {
            if (s1 == types::Symmetry::REFLECTION_IN_NESW || s1 == types::Symmetry::REFLECTION_IN_NWSE)
                continue;
            for (const auto s2: syms) {
                if (s2 == types::Symmetry::REFLECTION_IN_NESW || s2 == types::Symmetry::REFLECTION_IN_NWSE)
                    continue;
                const auto sc = types::composeSymmetries(s1, s2);
                if (sc == types::Symmetry::REFLECTION_IN_NESW || sc == types::Symmetry::REFLECTION_IN_NWSE)
                    continue;

                for (const auto gen: gens) {
                    const auto m = gen->generate();
                    const auto ms = m.applySymmetry(s1).applySymmetry(s2);
                    const auto mc = m.applySymmetry(sc);
                    REQUIRE(ms == mc);
                }
            }
        }
    }

    SECTION("Attempts to operate on a rectangular ThickMaze with diagonal symmetries should cause an exception") {
        for (const auto s1: syms) {
            for (const auto s2: syms) {
                const auto sc = types::composeSymmetries(s1, s2);
                if (s1 != types::Symmetry::REFLECTION_IN_NESW && s1 != types::Symmetry::REFLECTION_IN_NWSE &&
                    s2 != types::Symmetry::REFLECTION_IN_NESW && s2 != types::Symmetry::REFLECTION_IN_NWSE &&
                    sc != types::Symmetry::REFLECTION_IN_NESW && sc != types::Symmetry::REFLECTION_IN_NWSE)
                    continue;
                for (const auto gen: gens) {
                    const auto m = gen->generate();
                    REQUIRE_THROWS(m.applySymmetry(s1).applySymmetry(s2) == m.applySymmetry(sc));
                }
            }
        }
    }

    // Delete the generators.
    deleteThickMazeGenerators(gens);
}

TEST_CASE("Square ThickMazes can be manipulated via all symmetries", "[thickmaze][symmetry][square]") {
    constexpr auto side = 50;
    const auto dim = types::Dimensions2D{side, side};

    // ThickMaze constructions from regular mazes produce size 2w-1 by 2h-1.
    const auto dim_1 = types::Dimensions2D{side-1, side-1};

    // Get all the generators and all the symmetries.
    auto gens = createThickMazeGenerators(dim);
    const auto syms = types::symmetries();

    SECTION("All generators produce ThickMazes of the correct size: " +
            typeclasses::Show<types::Dimensions2D>::show(dim) + " or " +
            typeclasses::Show<types::Dimensions2D>::show(dim_1)) {
        for (const auto gen: gens) {
            const auto m = gen->generate();

            // Use approx to allow width / height or width - 1 / height - 1 (for the hommorphism constructions).
            REQUIRE(m.getWidth()  == Approx(side - 0.5).epsilon(0.5));
            REQUIRE(m.getHeight() == Approx(side - 0.5).epsilon(0.5));
        }
    }

    SECTION("A ThickMaze under the action of two symmetries equals the ThickMaze under the action of their composition") {
        for (const auto s1: syms) {
            for (const auto s2: syms) {
                const auto sc = types::composeSymmetries(s1, s2);

                for (const auto gen: gens) {
                    const auto m = gen->generate();
                    const auto ms = m.applySymmetry(s1).applySymmetry(s2);
                    const auto mc = m.applySymmetry(sc);
                    REQUIRE(ms == mc);
                }
            }
        }
    }

    // Delete the generators.
    deleteThickMazeGenerators(gens);
}

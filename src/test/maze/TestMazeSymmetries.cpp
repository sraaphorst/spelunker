#include <catch.hpp>

#include <string>

#include <types/Dimensions2D.h>
#include <types/Symmetry.h>
#include <typeclasses/Show.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>
#include "MazeGenerators.h"

using namespace spelunker;

TEST_CASE("Rectangular mazes can be manipulated via certain symmetries", "[maze][symmetry][rectangle]") {
    int constexpr width = 50;
    int constexpr height = 40;
    auto const dim = types::Dimensions2D{width, height};
    auto gens = createGenerators(dim);

    SECTION("All generators produce mazes of the correct size: " + typeclasses::Show<types::Dimensions2D>::show(dim)) {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            REQUIRE(m.getWidth() == width);
            REQUIRE(m.getHeight() == height);
        }
    }

    SECTION("Rotating by 90 degrees changes the maze") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m90 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_90);
            REQUIRE(m.getWidth() == m90.getHeight());
            REQUIRE(m.getHeight() == m90.getWidth());
            REQUIRE(m != m90);
        }
    }

    SECTION("Rotating twice by 90 degrees is the same as rotating by 180 degrees") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m90_90 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90);
            const maze::Maze m180 = m.applySymmetry(types::Symmetry::ROTATION_BY_180);

            REQUIRE(m90_90 == m180);
        }
    }

    SECTION("Rotating three times by 90 degrees is the same as rotating by 270 degrees") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m90_90_90 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90);
            const maze::Maze m270 = m.applySymmetry(types::Symmetry::ROTATION_BY_270);

            REQUIRE(m90_90_90 == m270);
        }
    }

    SECTION("Rotating four times by 90 degrees is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m90_90_90_90 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90)
                    .applySymmetry(types::Symmetry::ROTATION_BY_90);
            REQUIRE(m90_90_90_90 == m);
        }
    }

    SECTION("Rotating twice by 180 degrees is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m180_180 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_180)
                    .applySymmetry(types::Symmetry::ROTATION_BY_180);
            REQUIRE(m180_180 == m);
        }
    }

    SECTION("Rotating twice by 270 degrees is the same as rotating by 180 degrees") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m270_270 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270);
            const maze::Maze m180 = m.applySymmetry(types::Symmetry::ROTATION_BY_180);
            REQUIRE(m270_270 == m180);
        }
    }

    SECTION("Rotating three times by 270 degrees is the same as rotating by 90 degrees") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m270_270_270 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270);
            const maze::Maze m90 = m.applySymmetry(types::Symmetry::ROTATION_BY_90);
            REQUIRE(m270_270_270 == m90);
        }
    }

    SECTION("Rotating four times by 270 degrees is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze m270_270_270_270 = m
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270)
                    .applySymmetry(types::Symmetry::ROTATION_BY_270);
            REQUIRE(m270_270_270_270 == m);
        }
    }

    SECTION("Reflecting twice in the x-axis is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze mX_X = m
                    .applySymmetry(types::Symmetry::REFLECTION_IN_X)
                    .applySymmetry(types::Symmetry::REFLECTION_IN_X);
            REQUIRE(mX_X == m);
        }
    }

    SECTION("Reflecting twice in the y-axis is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze mY_Y = m
                    .applySymmetry(types::Symmetry::REFLECTION_IN_Y)
                    .applySymmetry(types::Symmetry::REFLECTION_IN_Y);
            REQUIRE(mY_Y == m);
        }
    }

    SECTION("Reflecting in the NESW axis for a non-square matrix generates an exception") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            REQUIRE_THROWS(m.applySymmetry(types::Symmetry::REFLECTION_IN_NESW));
        }
    }

    SECTION("Reflecting in the NWSE axis for a non-square matrix generates an exception") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            REQUIRE_THROWS(m.applySymmetry(types::Symmetry::REFLECTION_IN_NWSE));
        }
    }

    // Delete the generators.
    deleteGenerators(gens);
}

TEST_CASE("Square mazes can be manipulated via certain symmetries", "[maze][symmetry][square]") {
    int constexpr width = 50;
    int constexpr height = 50;
    auto const dim = types::Dimensions2D{width, height};
    auto gens = createGenerators(dim);

    SECTION("Reflecting twice in the NESW axis is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze mNESW_NESW = m
                    .applySymmetry(types::Symmetry::REFLECTION_IN_NESW)
                    .applySymmetry(types::Symmetry::REFLECTION_IN_NESW);
            REQUIRE(mNESW_NESW == m);
        }
    }

    SECTION("Reflecting twice in the NWSE axis is idempotent") {
        for (const auto gen: gens) {
            const maze::Maze m = gen->generate();
            const maze::Maze mNWSE_NWSE = m
                    .applySymmetry(types::Symmetry::REFLECTION_IN_NWSE)
                    .applySymmetry(types::Symmetry::REFLECTION_IN_NWSE);
            REQUIRE(mNWSE_NWSE == m);
        }
    }

    // Delete the generators.
    deleteGenerators(gens);
}

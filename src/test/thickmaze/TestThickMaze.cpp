/**
 * TestThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <sstream>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <maze/DFSMazeGenerator.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeAttributes.h>
#include <thickmaze/ThickMazeGeneratorByHomomorphism.h>

using namespace spelunker;

TEST_CASE("ThicMaze should be able to serialize and deserialize", "[thickmaze][serialization]") {
    constexpr auto width = 25;
    constexpr auto height = 20;
    const types::Dimensions2D dim{width, height};
    thickmaze::ThickMazeGeneratorByHomomorphism dfs{new maze::DFSMazeGenerator{dim}};

    const auto tm1 = dfs.generate();
    const auto tm2 = dfs.generate();

    std::stringstream ss1;
    std::stringstream ss2;

    tm1.save(ss1);
    tm2.save(ss2);

    // There is an incredibly small chance that this could fail.
    const auto tml1 = thickmaze::ThickMaze::load(ss1);
    const auto tml2 = thickmaze::ThickMaze::load(ss2);

    REQUIRE(tm1 != tm2);
    REQUIRE(tm1 == tml1);
    REQUIRE(tm2 == tml2);
    REQUIRE(tml1 != tml2);
}

TEST_CASE("ThickMaze should obey the rules of reversiblity") {
    constexpr auto width = 100;
    constexpr auto height = 50;

    thickmaze::CellularAutomatonThickMazeGenerator gen{width, height};
    auto tm = gen.generate();

    SECTION("Reverse on a maze with no start or goals should be an order 2 operation") {
        const auto tmr = tm.reverse();
        REQUIRE(tm != tmr);

        const auto tmrr = tmr.reverse();
        REQUIRE(tm == tmrr);
    }

    SECTION("Reverse should eliminate start positions and goals") {
        constexpr auto maxSpecialCells = 4;

        // Just find a start cell and two empty goal cells.
        types::CellCollection goals;

        auto num = 0;
        for (auto x = 0; x < width && num < maxSpecialCells; ++x) {
            for (auto y = 0; y < height && num < maxSpecialCells; ++y) {
                if (tm.cellIs(x, y) == thickmaze::CellType::FLOOR) {
                    if (num == 0) {
                        tm.setStartingCell(types::cell(x, y));
                        ++num;
                    } else {
                        goals.emplace_back(types::cell(x, y));
                        ++num;
                    }
                }
            }
        }
        tm.setGoalCells(goals);

        // These may not appear to be best practices, but getting Catch2 to compile
        // with boost::optional was not easy; hence the reliance on the deprecated method.
        REQUIRE(tm.getStartingCell().is_initialized());
        REQUIRE(tm.getGoalCells().size() > 0);
        const thickmaze::ThickMaze tmr = tm.reverse();
        REQUIRE_FALSE(tmr.getStartingCell().is_initialized());
        REQUIRE(tmr.getGoalCells().empty());
    }

}
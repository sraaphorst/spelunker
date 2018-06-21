/**
 * TestThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <types/Dimensions2D.h>
#include <maze/DFSMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
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
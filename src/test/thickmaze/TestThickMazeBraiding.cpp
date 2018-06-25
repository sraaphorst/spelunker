
#include <catch.hpp>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <maze/DFSMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeGeneratorByHomomorphism.h>

using namespace spelunker;

TEST_CASE("ThickMaze should be able to serialize and deserialize", "[thickmaze][serialization]") {
    constexpr auto width = 25;
    constexpr auto height = 20;
    const types::Dimensions2D dim{width, height};
    thickmaze::ThickMazeGeneratorByHomomorphism dfs{maze::DFSMazeGenerator{dim}};

    const thickmaze::ThickMaze tm  = dfs.generate();
    const thickmaze::ThickMaze tmb = tm.braidAll();
    REQUIRE(tm != tmb);

    const thickmaze::ThickMaze tmb2 = tmb.braidAll();
    REQUIRE(tmb == tmb2);
}
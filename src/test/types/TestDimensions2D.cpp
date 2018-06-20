/**
 * TestDimensions2D.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <types/Dimensions2D.h>
#include <types/Exceptions.h>

using namespace boost::archive;
using namespace spelunker;

static constexpr auto width  = 50;
static constexpr auto height = 40;

#include <iostream>

TEST_CASE("Dimensions2D should be able to serialize and deserialize", "[types][dimensions][serialization]") {
    const types::Dimensions2D dim{width, height};

    std::stringstream ss;
    dim.save(ss);

    const types::Dimensions2D dimL = types::Dimensions2D::load(ss);

    REQUIRE(dim == dimL);
}

TEST_CASE("Dimensions2D should not allow negative values", "[types][dimensions][illegal]") {
    SECTION("Width cannot be -1") {
        REQUIRE_THROWS(types::Dimensions2D{-1, height});
    }
    SECTION("Height cannot be -1") {
        REQUIRE_THROWS(types::Dimensions2D{width, -1});
    }
}
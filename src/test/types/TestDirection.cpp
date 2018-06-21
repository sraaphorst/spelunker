/**
 * TestDirection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <types/Direction.h>

using namespace boost::archive;
using namespace spelunker;
using namespace spelunker::types;

TEST_CASE("Directions should be able to serialize and deserialize", "[types][direction][serialization]") {
    std::stringstream ss;

    boost::archive::text_oarchive oa{ss};
    for (auto d: types::directions())
        oa << d;

    boost::archive::text_iarchive ia{ss};
    for (auto d: types::directions()) {
        types::Direction di;
        ia >> di;
        REQUIRE(d == di);
    }
}

TEST_CASE("Directions flipped twice should return to their original form", "[types][direction][group]") {
    for (auto d: types::directions())
        REQUIRE(flip(flip(d)) == d);
}
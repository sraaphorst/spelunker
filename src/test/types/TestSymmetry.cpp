/**
 * TestSymmetry.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <types/Direction.h>
#include <types/Symmetry.h>

using namespace boost::archive;
using namespace spelunker;
using namespace spelunker::types;

TEST_CASE("Symmetry should be able to serialize and deserialize", "[types][symmetry][serialization]") {
    std::stringstream ss;

    boost::archive::text_oarchive oa{ss};
    for (auto s: types::symmetries())
        oa << s;

    boost::archive::text_iarchive ia{ss};
    for (auto s: types::symmetries()) {
        types::Symmetry si;
        ia >> si;
        REQUIRE(s == si);
    }
}

TEST_CASE("Symmetry group has elements all of order 1, 2, or 4", "[types][symmetry][group]") {
    const types::Symmetry s2[5] {
            Symmetry::ROTATION_BY_180,
            Symmetry::REFLECTION_IN_X,
            Symmetry::REFLECTION_IN_Y,
            Symmetry::REFLECTION_IN_NESW,
            Symmetry::REFLECTION_IN_NWSE
    };

    const types::Symmetry s4[2] {
            Symmetry::ROTATION_BY_90,
            Symmetry::ROTATION_BY_270
    };

    SECTION("Rotation by 180 and all reflections have order 2") {
        for (const auto s: s2) {
            REQUIRE(s != Symmetry::IDENTITY);
            REQUIRE(composeSymmetries(s, s) == Symmetry::IDENTITY);
        }
    }

    SECTION("Rotation by 90 and 270 have order 4") {
        for (const auto s: s4) {
            Symmetry sl = s;
            Symmetry sr = s;
            for (int i=0; i < 3; ++i) {
                REQUIRE(sl != Symmetry::IDENTITY);
                REQUIRE(sr != Symmetry::IDENTITY);
                sl = composeSymmetries(s, sl);
                sr = composeSymmetries(sr, s);
            }
            REQUIRE(sl == Symmetry::IDENTITY);
            REQUIRE(sr == Symmetry::IDENTITY);
        }
    }

    SECTION("Identity acts properly on directions") {
        for (const auto d: directions())
            REQUIRE(applySymmetryToDirection(Symmetry::IDENTITY, d) == d);
    }

    SECTION("Symmetries of order 2 act properly on directions") {
        // Note that some symmetries of order 2 don't affect all directions.
        for (const auto d: directions())
            for (const auto s: s2)
                REQUIRE(applySymmetryToDirection(s, applySymmetryToDirection(s, d)) == d);
    }

    SECTION("Symmetries of order 4 act properly on directions") {
        for (const auto d: directions()) {
            for (const auto s: s4) {
                auto ds = applySymmetryToDirection(s, d);
                for (auto i = 0; i < 3; ++i) {
                    REQUIRE(ds != d);
                    ds = applySymmetryToDirection(s, ds);
                }
                REQUIRE(ds == d);
            }
        }
    }
}
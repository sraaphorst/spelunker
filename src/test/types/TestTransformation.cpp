/**
 * TestTransformation.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <types/Direction.h>
#include <types/Transformation.h>

using namespace boost::archive;
using namespace spelunker;
using namespace spelunker::types;

TEST_CASE("Transformation should be able to serialize and deserialize", "[types][transformation][serialization]") {
    std::stringstream ss;

    boost::archive::text_oarchive oa{ss};
    for (auto t: types::transformations())
        oa << t;

    boost::archive::text_iarchive ia{ss};
    for (auto t: types::transformations()) {
        types::Transformation ti;
        ia >> ti;
        REQUIRE(t == ti);
    }
}

TEST_CASE("Transformation group has elements all of order 1, 2, or 4", "[types][transformation][group]") {
    const types::Transformation t2[5] {
            Transformation::ROTATION_BY_180,
            Transformation::REFLECTION_IN_X,
            Transformation::REFLECTION_IN_Y,
            Transformation::REFLECTION_IN_NESW,
            Transformation::REFLECTION_IN_NWSE
    };

    const types::Transformation t4[2] {
            Transformation::ROTATION_BY_90,
            Transformation::ROTATION_BY_270
    };

    SECTION("Rotation by 180 and all reflections have order 2") {
        for (const auto t: t2) {
            REQUIRE(t != Transformation::IDENTITY);
            REQUIRE(composeTransformations(t, t) == Transformation::IDENTITY);
        }
    }

    SECTION("Rotation by 90 and 270 have order 4") {
        for (const auto t: t4) {
            Transformation tl = t;
            Transformation tr = t;
            for (int i=0; i < 3; ++i) {
                REQUIRE(tl != Transformation::IDENTITY);
                REQUIRE(tr != Transformation::IDENTITY);
                tl = composeTransformations(t, tl);
                tr = composeTransformations(tr, t);
            }
            REQUIRE(tl == Transformation::IDENTITY);
            REQUIRE(tr == Transformation::IDENTITY);
        }
    }

    SECTION("Identity acts properly on directions") {
        for (const auto d: directions())
            REQUIRE(applyTransformationToDirection(Transformation::IDENTITY, d) == d);
    }

    SECTION("Transformations of order 2 act properly on directions") {
        // Note that some transformations of order 2 don't affect all directions.
        for (const auto d: directions())
            for (const auto t: t2)
                REQUIRE(applyTransformationToDirection(t, applyTransformationToDirection(t, d)) == d);
    }

    SECTION("Transformations of order 4 act properly on directions") {
        for (const auto d: directions()) {
            for (const auto t: t4) {
                auto dt = applyTransformationToDirection(t, d);
                for (auto i = 0; i < 3; ++i) {
                    REQUIRE(dt != d);
                    dt = applyTransformationToDirection(t, dt);
                }
                REQUIRE(dt == d);
            }
        }
    }
}
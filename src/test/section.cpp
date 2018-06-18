#include "catch.hpp"

#include <vector>


// *** NOTE ***
// For each SECTION, the TEST_CASE is executed from the start.
// The changes are not cumulative across the SECTIONs: hence why in the last test, we check capacity >= 5 and not
//   capacity >= 10.
TEST_CASE("vectors can be sized and resized", "[vector]") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity()>= 5);

    SECTION("resizing bigger changes size and capacity") {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }

    SECTION("resizing smaller changes size but not capacity") {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);
    }

    SECTION("reserving bigger changes capacity but not size") {
        v.reserve(10);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);

        // SECTIONS can be nested!
        // Each leaf will be executed exactly once from start path to end.
        SECTION("reserving smaller again does not change capacity") {
            v.reserve(7);

            REQUIRE(v.capacity() >= 10);
        }
    }
}

// In line with BDD, we can rewrite the previous tests as follows:
SCENARIO("BDD: vectors can be sized and resized", "[vector]") {

    GIVEN("A vector with some items") {
        std::vector<int> v(5);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);

        WHEN("the size is increased") {
            v.resize(10);

            THEN ("the size and capacity change") {
                REQUIRE(v.size() == 10);
                REQUIRE(v.capacity() >= 10);
            }
        }

        WHEN("the size is reduced") {
            v.resize(0);

            THEN("the size changes but not capacity") {
                REQUIRE(v.size() == 0);
                REQUIRE(v.capacity() >= 5);
            }
        }

        WHEN("more capcity is reserved") {
            v.reserve(10);

            THEN("the capacity changes, but not the size") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 10);
            }
        }

        WHEN("less capacity is reserved") {
            v.reserve(0);

            THEN("neither size nor capacity are changed") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 5);
            }
        }
    }
}
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

TEST_CASE("Dimensions2D should be able to serialize and deserialize", "[types][dimensions][serialization]") {
    const types::Dimensions2D dim{width, height};

    std::stringstream ss;
    dim.save(ss);

    const types::Dimensions2D dimL = types::Dimensions2D::load(ss);

    REQUIRE(dim == dimL);
}

TEST_CASE("Dimensions2D should have overloaded operators", "[types][dimensions][operators]") {
    const types::Dimensions2D dim{width, height};

    SECTION("A Dimensions2D should be == to itself") {
        REQUIRE(dim == dim);
    }

    SECTION("The == operator should be overloaded to other objects") {
        const types::Dimensions2D dim2{width, height};
        REQUIRE(dim == dim2);
    }

    SECTION("The != operator should be overloaded") {
        const types::Dimensions2D dim2{width+1, height};
        REQUIRE(dim != dim2);
    }

    SECTION("Multiplication should be overloaded") {
        static constexpr auto factor = 2;
        const types::Dimensions2D dim2 = factor * dim;
        REQUIRE(dim2.getWidth() == factor * width);
        REQUIRE(dim2.getHeight() == factor * height);
    }

    SECTION("Addition should be overloaded") {
        static constexpr auto deltax = 1;
        static constexpr auto deltay = 2;
        const types::Dimensions2D dim2 = dim + types::Dimensions2D{deltax, deltay};
        REQUIRE(types::Dimensions2D{width + deltax, height + deltay} == dim2);
    }

    SECTION("Division should be overloaded") {
        static constexpr auto denom = 5;
        types::Dimensions2D dimby5 = dim / denom;
        REQUIRE(dimby5.getWidth() == width / denom);
        REQUIRE(dimby5.getHeight() == height / denom);
    }

    SECTION("Mixed operations should work correctly") {
        REQUIRE((dim + 3 * dim) / 2 == 2 * dim);
    }
}

TEST_CASE("Dimensions should be able to tell if a cell is in bounds", "[types][dimensions][exception]") {
    const types::Dimensions2D dim{width, height};

    SECTION("Points in bounds should be recognized as such") {
        for (auto x=0; x < width; ++x)
            for (auto y=0; y < height; ++y)
                REQUIRE(dim.cellInBounds(x, y));
    }

    SECTION("Points out of bounds should be recognized as such") {
        const int idxx[6] = {-3, -2, -1, width, width+1, width+2};
        const int idxy[6] = {-3, -2, -1, height, height+1, height+2};

        for (auto x=0; x < width; ++x)
            for (auto y: idxy)
                REQUIRE_FALSE(dim.cellInBounds(x, y));

        for (auto x: idxx)
            for (auto y=0; y < height; ++y)
                REQUIRE_FALSE(dim.cellInBounds(x, y));

        for (auto x: idxx)
            for (auto y: idxy)
                REQUIRE_FALSE(dim.cellInBounds(x, y));
    }

    SECTION("Checking a cell out of bounds should throw an exception") {
        REQUIRE_THROWS(dim.checkCell(width, 0));
        REQUIRE_THROWS(dim.checkCell(0, height));
        REQUIRE_THROWS(dim.checkCell(width, height));
        REQUIRE_THROWS(dim.checkCell(-1, height));
        REQUIRE_THROWS(dim.checkCell(width, -1));
    }
}

TEST_CASE("Dimensions should know if they are square", "[types][dimensions][square]") {
    for (auto x = 0; x < width; ++x)
        for (auto y=0; y < height; ++y)
            REQUIRE(types::Dimensions2D{x, y}.isSquare() == (x == y));
}

TEST_CASE("Dimensions2D should not allow negative values", "[types][dimensions][exception]") {
    SECTION("Width cannot be -1") {
        REQUIRE_THROWS(types::Dimensions2D{-1, height});
    }
    SECTION("Height cannot be -1") {
        REQUIRE_THROWS(types::Dimensions2D{width, -1});
    }
}

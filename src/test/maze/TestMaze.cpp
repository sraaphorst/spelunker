#include <catch.hpp>

int fib(int term) {
    if (term < 2)
        return 1;
    return fib(term-1) + fib(term-2);
}

static constexpr int fibs[] {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
static constexpr auto fibsNum = sizeof(fibs)/sizeof(int);

TEST_CASE("Test the lazy fibonacci", "[fibonacci]") {
    for (auto i=0; i < fibsNum; ++i)
        REQUIRE(fib(i) == fibs[i]);
}

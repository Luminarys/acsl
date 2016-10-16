//
// Created by luminarys on 10/15/16.
//
#include "catch.hh"
#include "acsl/range.hh"
#include "acsl/vector.hh"

using namespace acsl;

TEST_CASE("Range", "[range]") {
    Vector<int> v = {1, 2, 3, 4, 5};
    auto iter = v.iter();
    static_assert(IsBidirectionalRange<typeof(iter), int>);

    int start = 1;
    for (int& i: v.iter()) {
        REQUIRE(i == start++);
    }

    REQUIRE(iter.front().unwrap() == 1);
    iter.popFront();
    REQUIRE(iter.front().unwrap() == 2);
    iter.front().unwrap().get() = 0;
    REQUIRE(iter.front().unwrap() == 0);

    auto citer = v.citer();
    REQUIRE(citer.front().unwrap() == 1);
}

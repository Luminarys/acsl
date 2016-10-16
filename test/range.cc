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
    REQUIRE(iter.front().unwrap() == 1);
    iter.popFront();
    REQUIRE(iter.front().unwrap() == 2);
    // static_assert(IsInputRange<int, typeof(iter)>);
}

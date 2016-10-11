#include "test/catch.hh"
#include "acsl/vector.hh"

using namespace acsl;

TEST_CASE("Vector", "[vector]") {
    Vector<int> v = {1, 2, 3, 4, 5};
    REQUIRE(v.pop().unwrap() == 5);
    v.push(6);
    REQUIRE(v.pop().unwrap() == 6);
    REQUIRE(v[0].unwrap() == 1);
    REQUIRE(v[2].unwrap() == 3);
    v.insert(0, 10);
    REQUIRE(v.first().unwrap() == 10);
    REQUIRE(v.last().unwrap() == 4);


    Vector<int> v1 = v;
    REQUIRE(v1.pop().unwrap() == 4);
}


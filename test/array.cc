#include "test/catch.hh"
#include "acsl/array.hh"

using namespace acsl;

TEST_CASE("Array", "[array]") {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    REQUIRE(a.at<0>() == 1);
    REQUIRE(a.at<4>() == 5);
    a[0] = 5;
    REQUIRE(a[0] == 5);
    REQUIRE(a.at<0>() == 5);

    REQUIRE(a.front() == 5);
    REQUIRE(a.back() == 5);
}
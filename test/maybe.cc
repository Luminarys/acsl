#include "test/catch.hh"
#include "acsl/maybe.hh"

using namespace acsl;

TEST_CASE( "Maybe", "[maybe]" ) {
    auto mi = new Maybe<int>();
    REQUIRE(mi->is_none());
    REQUIRE(!mi->has_value());
    REQUIRE(mi->unwrap_or(5) == 5);

    mi = new Maybe<int>(5);
    REQUIRE(mi->unwrap_or(0) == 5);
    auto mn = mi->map<bool>([] (int i) { return true; });
    REQUIRE(mn.unwrap_or(false));

    mi = new Maybe<int>(5);
    REQUIRE(mi->map_or<bool>(false, [] (int i) { return i == 5; }));

    mi = new Maybe<int>(5);
    REQUIRE(mi->and_then<bool>([] (int i) { return Maybe<bool>(true); }).unwrap_or(false));
}

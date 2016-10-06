#include "test/catch.hh"
#include <assert.h>
#include "acsl/maybe.hh"

using namespace acsl;

TEST_CASE( "Maybe", "[maybe]" ) {
    auto mi = new Maybe<int>();
    REQUIRE(mi->is_none());
    REQUIRE(!mi->has_value());
    REQUIRE(mi->unwrap_or(5) == 5);
    mi = new Maybe<int>(5);
    REQUIRE(mi->unwrap_or(0) == 5);
    REQUIRE(mi->map([] (int i) { return true; }));
}

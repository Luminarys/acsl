#include "test/catch.hh"
#include "acsl/maybe.hh"

using namespace acsl;

Maybe<int> f() {
    return nothing;
}

TEST_CASE("Maybe", "[maybe]") {
    auto mi = f();
    REQUIRE(mi.is_none());
    REQUIRE_FALSE(mi.has_value());
    REQUIRE(mi.unwrap_or(5) == 5);

    mi = Maybe<int>(5);
    REQUIRE(mi.unwrap_or(10) == 5);

    mi = Maybe<int>(5);
    auto mn = mi.map<bool>([](int i) { return true; });
    REQUIRE(mn.unwrap_or(false));

    mi = Maybe<int>(5);
    REQUIRE(mi.map_or<bool>(false, [](int i) { return i == 5; }));

    mi = Maybe<int>(5);
    REQUIRE(mi.and_then<bool>([](int i) { return Maybe<bool>(true); }).unwrap_or(false));
}

#include "test/catch.hh"
#include "acsl/maybe.hh"

using namespace acsl;

template<typename F>
using RV = decltype(std::declval<F>()(std::declval<int>()));

template<typename F>
RV<F> f(F&& func) {
    return func();
}

TEST_CASE("Maybe", "[maybe]") {
    Maybe<int> mi = nothing;
    REQUIRE(mi.is_none());
    REQUIRE_FALSE(mi.has_value());
    REQUIRE(mi.unwrap_or(5) == 5);

    mi = 5;
    REQUIRE(mi.unwrap_or(10) == 5);

    mi = 5;
    auto mn = mi.map([](int i) { return true; });
    REQUIRE(mn.unwrap_or(false));

    mi = 5;
    auto f = [](int i) { return i == 5; };
    REQUIRE(mi.map_or(false, f));

    mi = 5;
    REQUIRE(mi.and_then([](int i) { return Maybe<bool>(true); }).unwrap_or(false) == true);
}

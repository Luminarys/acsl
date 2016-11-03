#include "test/catch.hh"
#include "acsl/tuple.hh"

using namespace acsl;

TEST_CASE("Tuple", "[tuple]") {
  auto t = Tuple<int, float>(5, 0.5);
}

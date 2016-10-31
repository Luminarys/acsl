#include "test/catch.hh"
#include "acsl/maybe.hh"
#include "acsl/result.hh"

using namespace acsl;

TEST_CASE("Result", "[result]") {
  auto r = Result<int, bool>(5);
  REQUIRE(r.is_ok());
  REQUIRE(!r.is_err());
  REQUIRE(r.ok().has_value());

  r = Result<int, bool>(5);
  REQUIRE(r.unwrap_or(10) == 5);

  auto r2 = Result<int, float>(5);
  REQUIRE(r2.map<bool>([](int i) { return i == 5; }).unwrap_or(false));

  r = Result<int, bool>(5);
  bool res = r.and_then<float>([](int b) { return Result<float, bool>(false); }).err().unwrap_or(true);
  REQUIRE_FALSE(res);
}



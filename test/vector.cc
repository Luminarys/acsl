#include "test/catch.hh"
#include "acsl/vector.hh"

using namespace acsl;

TEST_CASE("Vector", "[vector]") {
  Vector<int> v = {1, 2, 3, 4, 5};
  REQUIRE(v.pop() == 5);
  v.push(6);
  REQUIRE(v.pop() == 6);
  REQUIRE(v[0].unwrap() == 1);
  REQUIRE(v[2].unwrap() == 3);
  v.insert(0, 10);
  REQUIRE(v.first().unwrap() == 10);
  REQUIRE(v.last().unwrap() == 4);

  Vector<int> v1 = v;
  REQUIRE(v1.pop().unwrap() == 4);

  Vector<int> v2 = Vector<int>(10, 1);
  REQUIRE(v2.pop() == 1);
  v2.insert(0, 2);
  REQUIRE(v2.first().unwrap() == 2);

  Vector<int> v3 = std::move(v2);
  REQUIRE(v3.first().unwrap() == 2);
  v3 = std::move(v);
  REQUIRE(v3.first().unwrap() == 10);

  Vector<int> v4 = Vector<int>();
  v4.push(10);
  REQUIRE(v4.pop() == 10);
}


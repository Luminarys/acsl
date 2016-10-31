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

  Vector<int> v4 = {1, 2, 3, 4, 5};
  auto i = v4.iter();
  REQUIRE(i.next().unwrap() == 1);
  REQUIRE(i.next().unwrap() == 2);
  auto i2 = v4.iter().map([](auto i) { return i * 2; });
  REQUIRE(i2.next().unwrap() == 2);
  REQUIRE(i2.next().unwrap() == 4);
  REQUIRE(i2.next().unwrap() == 6);

  auto i3 = v4.iter().map([](auto i) { return i * 2; }).map([](auto i) { return i / 3; });
  REQUIRE(i3.next().unwrap() - 2 / 3 < 0.01);
  REQUIRE(i3.next().unwrap() - 4 / 3 < 0.01);
  REQUIRE(i3.next().unwrap() - 2 < 0.01);
}


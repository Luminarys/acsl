#include "test/catch.hh"
#include "acsl/iterator.hh"
#include "acsl/vector.hh"

using namespace acsl;

TEST_CASE("Iterator", "[iterator]") {
  Vector<int> v = {1, 2, 3, 4, 5};
  auto i1 = v.iter();
  REQUIRE(i1.next().unwrap() == 1);
  REQUIRE(i1.next().unwrap() == 2);
  auto i2 = v.iter().map([](auto i) { return i * 2; });
  REQUIRE(i2.next().unwrap() == 2);
  REQUIRE(i2.next().unwrap() == 4);
  REQUIRE(i2.next().unwrap() == 6);

  auto i3 = v.iter().map([](auto i) { return i * 2; }).map([](auto i) { return i / 3; });
  REQUIRE(i3.next().unwrap() - 2 / 3 < 0.01);
  REQUIRE(i3.next().unwrap() - 4 / 3 < 0.01);
  REQUIRE(i3.next().unwrap() - 2 < 0.01);

  auto i4 = v.iter().rev();
  REQUIRE(i4.next().unwrap() == 5);
  REQUIRE(i4.next().unwrap() == 4);

  int counter = 1;
  for (auto i : v.iter()) {
    REQUIRE(i == counter++);
  }

  auto r = range(10);
  REQUIRE(r.next() == 0);
  REQUIRE(r.next() == 1);

  counter = 10;
  for (auto i : range(10, 1)) {
    REQUIRE(i == counter--);
  }

  char comp = 'a';
  for (auto c : range('a', 'z')) {
    REQUIRE(c == comp++);
  }

  REQUIRE(v.iter().count() == 5);
  REQUIRE(v.iter().last().unwrap() == 5);

  REQUIRE(v.iter().chain(v.iter()).count() == 10);

  counter = 0;
  for (int i : range(10).filter([](Ref<int> c) { return c.get() % 2 == 0; })) {
    REQUIRE(i == counter);
    counter += 2;
  }

  auto v2 = range(10).collect<Vector<int>>();
  REQUIRE(v2[0] == 0);

  Vector<int> v3 = {1, 2, 3, 4, 5};
  v3.iter().apply([](Ref<int> c) { c.get() *= 2; });
  REQUIRE(v3[0] == 2);
}

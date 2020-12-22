#include "color.h"

#include "doctest.h"

using raytrace::Color;

// Color tests

TEST_CASE("Color creation") {
  Color c = Color{-0.5, 0.4, 1.7};
  CHECK(c.r == -0.5);
  CHECK(c.g == 0.4);
  CHECK(c.b == 1.7);
}

TEST_CASE("Adding and subtracting colors") {
  Color c1{0.9, 0.6, 0.75};
  Color c2{0.7, 0.1, 0.25};

  SUBCASE("Adding colors") { CHECK(c1 + c2 == Color{1.6, 0.7, 1.0}); }
  SUBCASE("Subtracting colors") { CHECK(c1 - c2 == Color{0.2, 0.5, 0.5}); }
}

TEST_CASE("Multiplying a color by a scalar") {
  Color c{0.2, 0.3, 0.4};

  CHECK(c * 2 == Color{0.4, 0.6, 0.8});
  CHECK(2 * c == Color{0.4, 0.6, 0.8});
}

TEST_CASE("Multiplying colors") {
  Color c1{1.0, 0.2, 0.4};
  Color c2{0.9, 1.0, 0.1};
  CHECK(c1 * c2 == Color{0.9, 0.2, 0.04});
}

TEST_CASE("Comparing colors") {
  auto c1 = Color{1.0, 0.2, 0.4};
  CHECK(c1 == Color{1.0, 0.2, 0.4});
  CHECK(c1 != Color{1.0, 0.2, 0.0});
  CHECK(c1 != Color{0.0, 0.2, 0.4});
  CHECK(c1 != Color{1.0, 0.0, 0.4});
}

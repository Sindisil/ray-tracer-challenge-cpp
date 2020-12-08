#include "color.h"

#include "doctest.h"

using raytrace::Color;

// Color tests

TEST_CASE("Color creation") {
  Color c = Color{-.5f, .4f, 1.7f};
  CHECK(c.r == -.5f);
  CHECK(c.g == .4f);
  CHECK(c.b == 1.7f);
}

TEST_CASE("Adding and subtracting colors") {
  Color c1{.9f, .6f, .75};
  Color c2{.7f, .1f, .25f};

  SUBCASE("Adding colors") { CHECK(c1 + c2 == Color{1.6f, .7f, 1.0f}); }
  SUBCASE("Subtracting colors") { CHECK(c1 - c2 == Color{.2f, .5f, .5f}); }
}

TEST_CASE("Multiplying a color by a scalar") {
  Color c{.2f, .3f, .4f};

  CHECK(c * 2 == Color{.4f, .6f, .8f});
  CHECK(2 * c == Color{.4f, .6f, .8f});
}

TEST_CASE("Multiplying colors") {
  Color c1{1.0f, 0.2f, 0.4f};
  Color c2{0.9f, 1.0f, 0.1f};
  CHECK(c1 * c2 == Color{.9f, .2f, .04f});
}

TEST_CASE("Comparing colors") {
  auto c1 = Color{1.0f, 0.2f, 0.4f};
  CHECK(c1 == Color{1.0f, 0.2f, 0.4f});
  CHECK(c1 != Color{1.0f, 0.2f, 0.0f});
  CHECK(c1 != Color{0.0f, 0.2f, 0.4f});
  CHECK(c1 != Color{1.0f, 0.0f, 0.4f});
}

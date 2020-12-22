#include "lights.h"

#include "doctest.h"

#include "color.h"
#include "primitives.h"

using raytrace::Color;
using raytrace::Point;
using raytrace::PointLight;

TEST_CASE("A point light has position and intensity") {
  auto position = Point{0, 0, 0};
  auto intensity = Color{1, 1, 1};
  auto light = PointLight{position, intensity};
  REQUIRE(light.intensity == intensity);
  REQUIRE(light.position == position);
}

TEST_CASE("A default constructed PointLight is a white light at {0, 0, 0}") {
  auto l = PointLight{};
  CHECK(l.intensity == raytrace::colors::white);
  CHECK(l.position == Point{0.0, 0.0, 0.0});
}

TEST_CASE("PointLights can be compared") {
  auto p1 = PointLight{Point{1, 1, 1}, Color{1, 2, 3}};
  auto p2 = PointLight{Point{1, 1, 1}, Color{1, 2, 3}};
  auto p3 = PointLight{Point{0, 0, 0}, Color{1, 1, 1}};

  CHECK(p1 == p2);
  CHECK(p1 != p3);
}

#include "lights.h"

#include "doctest.h"

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

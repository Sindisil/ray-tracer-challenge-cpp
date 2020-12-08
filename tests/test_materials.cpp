#include "materials.h"

#include "doctest.h"

#include "primitives.h"

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::Material;

TEST_CASE("The default material") {
  auto m = Material{};
  CHECK(m.color() == Color{1, 1, 1});
  CHECK(are_about_equal(m.ambient(), 0.1f));
  CHECK(are_about_equal(m.diffuse(), 0.9f));
  CHECK(are_about_equal(m.specular(), 0.9f));
  CHECK(are_about_equal(m.shininess(), 200.0f));
}
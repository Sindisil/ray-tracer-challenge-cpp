#include "world.h"

#include "doctest.h"

#include "color.h"
#include "lights.h"
#include "primitives.h"
#include "sphere.h"

using raytrace::Color;
using raytrace::default_world;
using raytrace::identity_matrix;
using raytrace::Material;
using raytrace::Point;
using raytrace::PointLight;
using raytrace::Sphere;
using raytrace::World;

TEST_CASE("Newly constructed World is empty") {
  auto w = World{};
  CHECK(w.empty());
  CHECK(w.size() == 0);
  CHECK(w.begin() == w.end());
}

TEST_CASE("The default World") {
  auto l = PointLight{Point{-10.0f, 10.0f, -10.0f}, Color{1, 1, 1}};
  auto s1 =
      Sphere{Material{Color{0.8f, 1.0f, 0.6f}}.diffuse(0.7f).specular(0.2f)};
  auto s2 = Sphere{identity_matrix().scale(0.5f, 0.5f, 0.5f)};
  auto w = default_world();

  REQUIRE(!w.empty());
  REQUIRE(w.size() == 2);
  CHECK(w.light() == l);
  CHECK(w.contains(s1));
  CHECK(w.contains(s2));

  auto prev_id = unsigned{0};
  for (auto i : w) {
    CHECK(prev_id < i.id());
    prev_id = i.id();
  }
}
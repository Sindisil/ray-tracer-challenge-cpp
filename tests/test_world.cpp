#include "world.h"

#include "doctest.h"

#include "color.h"
#include "lights.h"
#include "primitives.h"
#include "ray.h"
#include "sphere.h"

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::default_world;
using raytrace::identity_matrix;
using raytrace::Intersection;
using raytrace::Material;
using raytrace::Point;
using raytrace::PointLight;
using raytrace::PreComps;
using raytrace::Ray;
using raytrace::Sphere;
using raytrace::Vec3;
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

TEST_CASE("Intersect a world with a ray") {
  auto w = default_world();
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vec3{0.0f, 0.0f, 1.0f}};
  auto xs = w.intersect(r);

  REQUIRE(xs.size() == 4);
  CHECK_EQ(xs[0].t, doctest::Approx(4.0f));
  CHECK_EQ(xs[1].t, doctest::Approx(4.5f));
  CHECK_EQ(xs[2].t, doctest::Approx(5.5f));
  CHECK_EQ(xs[3].t, doctest::Approx(6.0f));
}

TEST_CASE("Precomputing the state of an intersection") {
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vec3{0.0f, 0.0f, 1.0f}};
  auto shape = Sphere{};
  auto i = Intersection{4.0f, shape};
  auto comps = PreComps{i, r};

  CHECK(comps.intersection.t == i.t);
  CHECK(comps.intersection.object == i.object);
  CHECK(comps.point == Point{0.0f, 0.0f, -1.0f});
  CHECK(comps.eye_vec == Vec3{0.0f, 0.0f, -1.0f});
  CHECK(comps.normal == Vec3{0.0f, 0.0f, -1.0f});
}
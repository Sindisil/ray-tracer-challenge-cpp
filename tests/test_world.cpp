#include "world.h"

#include "doctest.h"

#include "color.h"
#include "lights.h"
#include "primitives.h"
#include "ray.h"
#include "shapes.h"

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::default_world;
using raytrace::epsilon;
using raytrace::identity_matrix;
using raytrace::Intersection;
using raytrace::Material;
using raytrace::Point;
using raytrace::PointLight;
using raytrace::PreComps;
using raytrace::Ray;
using raytrace::Sphere;
using raytrace::Vector3;
using raytrace::World;

namespace Colors = raytrace::colors;

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
  auto s2 = Sphere{identity_matrix().scaled(0.5f, 0.5f, 0.5f)};
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
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto xs = w.intersect(r);

  REQUIRE(xs.size() == 4);
  CHECK_EQ(xs[0].t, doctest::Approx(4.0f));
  CHECK_EQ(xs[1].t, doctest::Approx(4.5f));
  CHECK_EQ(xs[2].t, doctest::Approx(5.5f));
  CHECK_EQ(xs[3].t, doctest::Approx(6.0f));
}

TEST_CASE("Precomputing the state of an intersection") {
  auto shape = Sphere{};

  SUBCASE("Intersection on the outside") {
    auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
    auto i = Intersection{4.0f, shape};
    auto comps = PreComps{i, r};
    CHECK(comps.intersection().t == i.t);
    CHECK(comps.intersection().object == i.object);
    CHECK(comps.point() == Point{0.0f, 0.0f, -1.0f});
    CHECK(comps.eye_vec() == Vector3{0.0f, 0.0f, -1.0f});
    CHECK(comps.normal() == Vector3{0.0f, 0.0f, -1.0f});
    CHECK(!comps.inside());
  }

  SUBCASE("Intersection on the inside") {
    auto r = Ray{Point{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
    auto i = Intersection{1.0f, shape};
    auto comps = PreComps{i, r};
    CHECK(comps.point() == Point{0.0f, 0.0f, 1.0f});
    CHECK(comps.eye_vec() == Vector3{0.0f, 0.0f, -1.0f});
    CHECK(comps.inside());
    CHECK(comps.normal() == Vector3{0.0f, 0.0f, -1.0f});
  }

  SUBCASE("The hit should offset the point") {
    auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
    auto s = Sphere{}.transform(identity_matrix().translated(0.0f, 0.0f, 1.0f));
    auto i = Intersection{5, s};
    auto comps = PreComps{i, r};
    CHECK(comps.over_point().z < -epsilon / 2);
    CHECK(comps.point().z > comps.over_point().z);
  }
}

TEST_CASE("Shading an intersection") {
  auto w = default_world();
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto shape = w[0];
  auto i = Intersection{4, shape};
  auto comps = PreComps{i, r};
  auto c = w.shade_hit(comps);
  CHECK(c == Color{0.38066f, 0.47583f, 0.2855f});
}

TEST_CASE("Shading an intersection from the inside") {
  auto w = default_world().light(
      PointLight{Point{0.0f, 0.25f, 0.0f}, Color{1.0f, 1.0f, 1.0f}});
  auto r = Ray{Point{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto shape = w[1];
  auto i = Intersection{0.5f, shape};
  auto comps = PreComps{i, r};
  auto c = w.shade_hit(comps);
  CHECK(c == Color{0.90498f, 0.90498f, 0.90498f});
}

TEST_CASE("shade_hit is given an intersection in shadow") {
  auto w = World{};
  w.light(PointLight{Point{0.0f, 0.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}});
  auto s1 = Sphere{};
  auto s2 = Sphere{}.transform(identity_matrix().translated(0.0f, 0.0f, 10.0f));
  w.push_back(s1);
  w.push_back(s2);
  auto r = Ray{Point{0.0f, 0.0f, 5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto i = Intersection{4, s2};
  auto comps = PreComps{i, r};
  auto c = w.shade_hit(comps);
  CHECK_EQ(c, Color{0.1f, 0.1f, 0.1f});
}

TEST_CASE("The color when a ray misses") {
  auto w = default_world();
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 1.0f, 0.0f}};
  CHECK(w.color_at(r) == Colors::black);
}

TEST_CASE("The color when a ray hits") {
  auto w = default_world();
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  CHECK(w.color_at(r) == Color{0.38066f, 0.47583f, 0.2855f});
}

TEST_CASE("The color with an intersection behind the ray") {
  auto w = default_world();
  for (auto &o : w) {
    o.material().ambient(1.0f);
  }
  auto r = Ray{Point{0.0f, 0.0f, 0.75f}, Vector3{0.0f, 0.0f, -1.0f}};
  CHECK(w.color_at(r) == w[1].material().color());
}

TEST_CASE("There is no shadow when nothing is collinear with point and light") {
  auto w = default_world();
  auto p = Point{0.0f, 10.0f, 0.0f};
  CHECK(!w.is_shadowed(p));
}

TEST_CASE("The shadow when an object is between the point and the light") {
  auto w = default_world();
  auto p = Point{10.0f, -10.0f, 10.0f};
  CHECK(w.is_shadowed(p));
}

TEST_CASE("There is no shadowwhen an object is behind the light") {
  auto w = default_world();
  auto p = Point{-20.0f, 20.0f, -20.0f};
  CHECK(!w.is_shadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the point") {
  auto w = default_world();
  auto p = Point{-2.0f, -2.0f, -2.0f};
  CHECK(!w.is_shadowed(p));
}
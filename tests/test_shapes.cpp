#include "doctest.h"

#include "shapes.h"

#include <cmath>

#include "color.h"
#include "materials.h"
#include "primitives.h"
#include "ray.h"

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::identity_matrix;
using raytrace::Intersection;
using raytrace::Intersections;
using raytrace::Material;
using raytrace::pi;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Sphere;
using raytrace::Vector3;

TEST_CASE("A sphere's default transformation") {
  Sphere s;
  REQUIRE(s.transform() == identity_matrix());
}

TEST_CASE("Creating a sphere with a transformation") {
  auto t = identity_matrix().translated(2, 3, 4);
  Sphere s{t};
  REQUIRE(s.transform() == t);
}

TEST_CASE("Comparing spheres") {
  auto s1 = Sphere{};
  auto s2 = Sphere{};
  auto s3 = Sphere{identity_matrix().scaled(0.5f, 0.5f, 0.5f)};
  auto s4 = s1;
  CHECK(s1 == s1);
  CHECK(s1 != s3);
  CHECK(!s1.is(s2));
  CHECK(s1.is(s4));
}

TEST_CASE("Changing a sphere's transformation") {
  Sphere s;
  auto t = identity_matrix().translated(2, 3, 4);
  s.transform(t);
  REQUIRE(s.transform() == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vector3{0, 0, 1}};
  Sphere s{identity_matrix().scaled(2, 2, 2)};
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 3));
  CHECK(are_about_equal(xs[1].t, 7));
}

TEST_CASE("Intersecting a translated sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vector3{0, 0, 1}};
  Sphere s{identity_matrix().translated(5, 0, 0)};
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("The normal of a sphere at a point on the x axis") {
  Sphere s;
  auto n = s.normal_at(Point{1, 0, 0});
  CHECK(n == Vector3{1, 0, 0});
}

TEST_CASE("The normal on a sphere at a point on the y axis") {
  Sphere s;
  auto n = s.normal_at(Point{0, 1, 0});
  CHECK(n == Vector3{0, 1, 0});
}

TEST_CASE("The normal on a sphere at a point on the z axis") {
  Sphere s;
  auto n = s.normal_at(Point{0, 0, 1});
  CHECK(n == Vector3{0, 0, 1});
}

TEST_CASE("The normal on a sphere at a nonaxial point") {
  Sphere s;
  auto n = s.normal_at(Point{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                             std::sqrt(3.0f) / 3.0f});
  CHECK(n == Vector3{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                     std::sqrt(3.0f) / 3.0f});
}

TEST_CASE("The normal is a normalized vector") {
  Sphere s;
  auto n = s.normal_at(Point{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                             std::sqrt(3.0f) / 3.0f});
  CHECK(n == n.normalize());
}

TEST_CASE("Computing the normal on a translated sphere") {
  Sphere s(identity_matrix().translated(0, 1, 0));
  auto n = s.normal_at(Point{0, 1.70711f, -0.70711f});
  CHECK(n == Vector3{0, 0.70711f, -0.70711f});
}

TEST_CASE("Computing the normal on a transformed sphere") {
  Sphere s(identity_matrix().rotated_on_z(pi / 5.0f).scaled(1, 0.5f, 1));
  auto n = s.normal_at(Point{0, std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2});
  CHECK(n == Vector3{0, 0.97014f, -0.24254f});
}

TEST_CASE("A sphere has a default material") {
  auto s = Sphere{};
  CHECK(s.material() == Material());
}

TEST_CASE("A sphere may be created with a specified material") {
  auto m = Material{Color{1.0f, 1.0f, 1.0f}};
  auto s = Sphere{m};
  CHECK(s.material() == m);
}

TEST_CASE("A sphere may be assigned a material") {
  auto s = Sphere{};
  auto m = Material{};
  m.ambient(1.0f);
  s.material(m);
  CHECK(s.material() == m);
  m.ambient(.5f);
  CHECK(s.material() != m);
}

TEST_CASE("A ray intersects a sphere at two points") {
  Ray r{Point{0, 0, -5}, Vector3{0, 0, 1}};
  Sphere s{};

  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 4.0));
  CHECK(are_about_equal(xs[1].t, 6.0));
}

TEST_CASE("A ray intersects a sphere at a tangent") {
  Ray r{Point{0, 1, -5}, Vector3{0, 0, 1}};
  Sphere s{};
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 5.0));
  CHECK(are_about_equal(xs[1].t, 5.0));
}

TEST_CASE("A ray misses a sphere") {
  Ray r{Point{0, 2, -5}, Vector3{0, 0, 1}};
  Sphere s{};
  auto xs = s.intersect(r);
  REQUIRE(xs.empty());
}

TEST_CASE("A ray originates inside a sphere") {
  Ray r{Point{0, 0, 0}, Vector3{0, 0, 1}};
  Sphere s{};
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, -1.0));
  CHECK(are_about_equal(xs[1].t, 1.0));
}

TEST_CASE("A sphere is behind a ray") {
  Ray r{Point{0, 0, 5}, Vector3{0, 0, 1}};
  Sphere s{};
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, -6.0));
  CHECK(are_about_equal(xs[1].t, -4.0));
}

TEST_CASE("An intersection encapsulates t and object") {
  Sphere s;
  Intersection i{3.5f, &s};
  CHECK(are_about_equal(i.t, 3.5));
  CHECK(*i.object == s);
}

TEST_CASE("Aggregating intersections") {
  SUBCASE("Non-empty Intersections") {
    Sphere s;
    Intersection i1{1, &s};
    Intersection i2{2, &s};
    Intersections xs;
    xs.insert(i1);
    xs.insert(i2);
    REQUIRE(!xs.empty());
    REQUIRE(xs.size() == 2);
    CHECK(*xs[0].object == s);
    CHECK(*xs[1].object == s);
    for (auto const i : xs) {
      CHECK(*i.object == s);
    }
  }
  SUBCASE("Empty Intersections") {
    Intersections xs;
    CHECK(xs.empty());
    CHECK(std::begin(xs) == std::end(xs));
  }
}

TEST_CASE("Intersect sets the object on the intersection") {
  Ray r{Point{0, 0, -5}, Vector3{0, 0, 1}};
  Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  CHECK(*xs[0].object == s);
  CHECK(*xs[1].object == s);
}

TEST_CASE("The hit, when all intersections have positive t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{1, &s};
  auto i2 = Intersection{2, &s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i1);
}

TEST_CASE("The hit, when some intersections have negative t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{-1, &s};
  auto i2 = Intersection{1, &s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i2);
}

TEST_CASE("The hit, when all intersections have negative t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{-2, &s};
  auto i2 = Intersection{-1, &s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(!i.has_value());
}

TEST_CASE("The hit is always the lowest nonnegative intersection") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{5, &s};
  auto i2 = Intersection{7, &s};
  auto i3 = Intersection{-3, &s};
  auto i4 = Intersection{2, &s};
  xs.insert(i1);
  xs.insert(i2);
  xs.insert(i3);
  xs.insert(i4);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i4);
}

#include "doctest.h"

#include "sphere.h"

#include <cmath>

#include "color.h"
#include "materials.h"
#include "primitives.h"
#include "ray.h"

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::identity_matrix;
using raytrace::Material;
using raytrace::pi;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Sphere;
using raytrace::Vec3;

TEST_CASE("A sphere's default transformation") {
  Sphere s;
  REQUIRE(s.transform() == identity_matrix());
}

TEST_CASE("Creating a sphere with a transformation") {
  auto t = identity_matrix().translate(2, 3, 4);
  Sphere s{t};
  REQUIRE(s.transform() == t);
}

TEST_CASE("Comparing spheres") {
  auto s1 = Sphere{};
  auto s2 = Sphere{};
  auto s3 = Sphere{identity_matrix().scale(0.5f, 0.5f, 0.5f)};
  auto s4 = s1;
  CHECK(s1 == s1);
  CHECK(s1 != s3);
  CHECK(!s1.is(s2));
  CHECK(s1.is(s4));
}

TEST_CASE("Changing a sphere's transformation") {
  Sphere s;
  auto t = identity_matrix().translate(2, 3, 4);
  s.transform(t);
  REQUIRE(s.transform() == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s{identity_matrix().scale(2, 2, 2)};
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 3));
  CHECK(are_about_equal(xs[1].t, 7));
}

TEST_CASE("Intersecting a translated sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s{identity_matrix().translate(5, 0, 0)};
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("The normal of a sphere at a point on the x axis") {
  Sphere s;
  auto n = s.normal_at(Point{1, 0, 0});
  CHECK(n == Vec3{1, 0, 0});
}

TEST_CASE("The normal on a sphere at a point on the y axis") {
  Sphere s;
  auto n = s.normal_at(Point{0, 1, 0});
  CHECK(n == Vec3{0, 1, 0});
}

TEST_CASE("The normal on a sphere at a point on the z axis") {
  Sphere s;
  auto n = s.normal_at(Point{0, 0, 1});
  CHECK(n == Vec3{0, 0, 1});
}

TEST_CASE("The normal on a sphere at a nonaxial point") {
  Sphere s;
  auto n = s.normal_at(Point{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                             std::sqrt(3.0f) / 3.0f});
  CHECK(n == Vec3{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                  std::sqrt(3.0f) / 3.0f});
}

TEST_CASE("The normal is a normalized vector") {
  Sphere s;
  auto n = s.normal_at(Point{std::sqrt(3.0f) / 3.0f, std::sqrt(3.0f) / 3.0f,
                             std::sqrt(3.0f) / 3.0f});
  CHECK(n == n.normalize());
}

TEST_CASE("Computing the normal on a translated sphere") {
  Sphere s(identity_matrix().translate(0, 1, 0));
  auto n = s.normal_at(Point{0, 1.70711f, -0.70711f});
  CHECK(n == Vec3{0, 0.70711f, -0.70711f});
}

TEST_CASE("Computing the normal on a transformed sphere") {
  Sphere s(identity_matrix().rotate_z(pi / 5.0f).scale(1, 0.5f, 1));
  auto n = s.normal_at(Point{0, std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2});
  CHECK(n == Vec3{0, 0.97014f, -0.24254f});
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
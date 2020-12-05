#include "doctest.h"

#include "sphere.h"

#include "ray.h"

using raytrace::are_about_equal;
using raytrace::identity_matrix;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Sphere;
using raytrace::Vec3;

TEST_CASE("A sphere's default transformation") {
  Sphere s;
  REQUIRE(s.transform == identity_matrix());
}

TEST_CASE("Creating a sphere with a transformation") {
  auto t = identity_matrix().translate(2, 3, 4);
  Sphere s{t};
  REQUIRE(s.transform == t);
}

TEST_CASE("Changing a sphere's transformation") {
  Sphere s;
  auto t = identity_matrix().translate(2, 3, 4);
  s.transform = t;
  REQUIRE(s.transform == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s{identity_matrix().scale(2, 2, 2)};
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 3));
  CHECK(are_about_equal(xs[1].t, 7));
}

TEST_CASE("Intersecting a translated sphere with a ray") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s{identity_matrix().translate(5, 0, 0)};
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 0);
}

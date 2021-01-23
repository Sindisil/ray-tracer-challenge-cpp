#include "doctest.h"

#include "plane.h"

#include "intersections.h"
#include "primitives.h"
#include "ray.h"

using raytrace::Intersections;
using raytrace::Plane;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Vector3;

TEST_CASE("The normal of a plane is constant everywhere") {
  auto p = Plane{};
  auto n1 = p.local_normal_at(Point{0.0f, 0.0f, 0.0f});
  auto n2 = p.local_normal_at(Point{10.0f, 0.0f, -10.0f});
  auto n3 = p.local_normal_at(Point{-5.0f, 0.0f, 150.0f});
  CHECK(n1 == Vector3{0.0f, 01.0f, 0.0f});
  CHECK(n2 == Vector3{0.0f, 01.0f, 0.0f});
  CHECK(n3 == Vector3{0.0f, 01.0f, 0.0f});
}

TEST_CASE("Intersect with a ray parallel to the plane") {
  auto p = Plane{};
  auto r = Ray{Point{0.0f, 10.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto xs = Intersections{};
  p.local_intersect(r, xs);
  REQUIRE(xs.empty());
}

TEST_CASE("Intersect with a coplanar ray") {
  auto p = Plane{};
  auto r = Ray{Point{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto xs = Intersections{};
  p.local_intersect(r, xs);
  REQUIRE(xs.empty());
}

TEST_CASE("A ray intersecting a plane from above") {
  auto p = Plane{};
  auto r = Ray{Point{0.0f, 1.0f, 0.0f}, Vector3{0.0f, -1.0f, 0.0f}};
  auto xs = Intersections{};
  p.local_intersect(r, xs);
  REQUIRE(xs.size() == 1);
  CHECK(xs[0].t == 1);
  CHECK(xs[0].object->is(p));
}

TEST_CASE("A ray intersecting a plane from below") {
  auto p = Plane{};
  auto r = Ray{Point{0.0f, -1.0f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}};
  auto xs = Intersections{};
  p.local_intersect(r, xs);
  REQUIRE(xs.size() == 1);
  CHECK(xs[0].t == 1);
  CHECK(xs[0].object->is(p));
}

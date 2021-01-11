#include "doctest.h"

#include "ray.h"

#include <vector>

#include "matrix.h"
#include "primitives.h"
#include "shapes.h"

using raytrace::are_about_equal;
using raytrace::identity_matrix;
using raytrace::Matrix4;
using raytrace::Point;
using raytrace::Ray;
using raytrace ::Sphere;
using raytrace::Vector3;

TEST_CASE("creating and querying a ray") {
  Point origin{1, 2, 3};
  Vector3 direction{4, 5, 6};
  Ray r1{origin, direction};
  Ray r2{Point{1, 2, 3}, Vector3{4, 5, 6}};

  CHECK(r1.origin == origin);
  CHECK(r1.direction == direction);
  CHECK(r2.origin == origin);
  CHECK(r2.direction == direction);
}

TEST_CASE("Comparing rays") {
  auto r = Ray{Point{1, 2, 3}, Vector3{4, 5, 6}};
  CHECK(r == Ray{Point{1, 2, 3}, Vector3{4, 5, 6}});
  CHECK(r != Ray{Point{0, 0, 0}, Vector3{4, 5, 6}});
  CHECK(r != Ray{Point{1, 2, 3}, Vector3{0, 0, 1}});
}

TEST_CASE("Computing a point from a distance") {
  Ray r{Point{2, 3, 4}, Vector3{1, 0, 0}};
  CHECK(r.position(0) == Point{2, 3, 4});
  CHECK(r.position(1) == Point{3, 3, 4});
  CHECK(r.position(-1) == Point{1, 3, 4});
  CHECK(r.position(2.5) == Point{4.5, 3, 4});
}

TEST_CASE("Translating a ray") {
  Ray r{Point{1, 2, 3}, Vector3{0, 1, 0}};
  auto r2 = r.transform(identity_matrix().translated(3, 4, 5));
  CHECK(r2.origin == Point{4, 6, 8});
  CHECK(r2.direction == Vector3{0, 1, 0});
}

TEST_CASE("Scaling a ray") {
  Ray r{Point{1, 2, 3}, Vector3{0, 1, 0}};
  auto r2 = r.transform(identity_matrix().scaled(2, 3, 4));
  CHECK(r2.origin == Point{2, 6, 12});
  CHECK(r2.direction == Vector3{0, 3, 0});
}

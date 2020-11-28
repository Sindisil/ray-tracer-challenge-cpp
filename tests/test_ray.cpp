#include "doctest.h"

#include "ray.h"

#include "primatives.h"

using raytrace::Point;
using raytrace::Ray;
using raytrace::Vec3;

TEST_CASE("creating and querying a ray") {
  Point origin{1, 2, 3};
  Vec3 direction{4, 5, 6};
  Ray r1{origin, direction};
  Ray r2{Point{1, 2, 3}, Vec3{4, 5, 6}};

  CHECK(r1.origin == origin);
  CHECK(r1.direction == direction);
  CHECK(r2.origin == origin);
  CHECK(r2.direction == direction);
}

TEST_CASE("Computing a point from a distance") {
  Ray r{Point{2, 3, 4}, Vec3{1, 0, 0}};
  CHECK(r.position(0) == Point{2, 3, 4});
  CHECK(r.position(1) == Point{3, 3, 4});
  CHECK(r.position(-1) == Point{1, 3, 4});
  CHECK(r.position(2.5) == Point{4.5, 3, 4});
}
#include "doctest.h"

#include "ray.h"

#include <vector>

#include "primatives.h"
#include "sphere.h"

using raytrace::aboutEqual;
using raytrace::intersect;
using raytrace::Intersection;
using raytrace::intersections;
using raytrace::Point;
using raytrace::Ray;
using raytrace ::Sphere;
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

TEST_CASE("A ray intersects a sphere at two points") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s{};

  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(aboutEqual(xs[0].t, 4.0));
  CHECK(aboutEqual(xs[1].t, 6.0));
}

TEST_CASE("A ray intersects a sphere at a tangent") {
  Ray r{Point{0, 1, -5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(aboutEqual(xs[0].t, 5.0));
  CHECK(aboutEqual(xs[1].t, 5.0));
}

TEST_CASE("A ray misses a sphere") {
  Ray r{Point{0, 2, -5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = intersect(s, r);
  REQUIRE(xs.empty());
}

TEST_CASE("A ray originates inside a sphere") {
  Ray r{Point{0, 0, 0}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(aboutEqual(xs[0].t, -1.0));
  CHECK(aboutEqual(xs[1].t, 1.0));
}

TEST_CASE("A sphere is behind a ray") {
  Ray r{Point{0, 0, 5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(aboutEqual(xs[0].t, -6.0));
  CHECK(aboutEqual(xs[1].t, -4.0));
}

TEST_CASE("An intersection encapsulates t and object") {
  Sphere s;
  Intersection i{3.5f, s};
  CHECK(aboutEqual(i.t, 3.5));
  CHECK(i.object == s);
}

TEST_CASE("Aggregating intersections") {
  Sphere s;
  Intersection i1{1, s};
  Intersection i2{2, s};
  auto xs = std::vector<Intersection>({i1, i2});
  REQUIRE(xs.size() == 2);
  CHECK(xs[0].object == s);
  CHECK(xs[1].object == s);
}

TEST_CASE("Intersect sets the object on the intersection") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s;
  auto xs = intersect(s, r);
  REQUIRE(xs.size() == 2);
  CHECK(xs[0].object == s);
  CHECK(xs[1].object == s);
}
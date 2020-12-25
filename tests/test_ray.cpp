#include "doctest.h"

#include "ray.h"

#include <vector>

#include "matrix.h"
#include "primitives.h"
#include "sphere.h"

using raytrace::are_about_equal;
using raytrace::identity_matrix;
using raytrace::Intersection;
using raytrace::Intersections;
using raytrace::Mat4;
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

TEST_CASE("Comparing rays") {
  auto r = Ray{Point{1, 2, 3}, Vec3{4, 5, 6}};
  CHECK(r == Ray{Point{1, 2, 3}, Vec3{4, 5, 6}});
  CHECK(r != Ray{Point{0, 0, 0}, Vec3{4, 5, 6}});
  CHECK(r != Ray{Point{1, 2, 3}, Vec3{0, 0, 1}});
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

  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 4.0));
  CHECK(are_about_equal(xs[1].t, 6.0));
}

TEST_CASE("A ray intersects a sphere at a tangent") {
  Ray r{Point{0, 1, -5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, 5.0));
  CHECK(are_about_equal(xs[1].t, 5.0));
}

TEST_CASE("A ray misses a sphere") {
  Ray r{Point{0, 2, -5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = r.intersect(s);
  REQUIRE(xs.empty());
}

TEST_CASE("A ray originates inside a sphere") {
  Ray r{Point{0, 0, 0}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, -1.0));
  CHECK(are_about_equal(xs[1].t, 1.0));
}

TEST_CASE("A sphere is behind a ray") {
  Ray r{Point{0, 0, 5}, Vec3{0, 0, 1}};
  Sphere s{};
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(are_about_equal(xs[0].t, -6.0));
  CHECK(are_about_equal(xs[1].t, -4.0));
}

TEST_CASE("An intersection encapsulates t and object") {
  Sphere s;
  Intersection i{3.5f, s};
  CHECK(are_about_equal(i.t, 3.5));
  CHECK(i.object == s);
}

TEST_CASE("Aggregating intersections") {
  SUBCASE("Non-empty Intersections") {
    Sphere s;
    Intersection i1{1, s};
    Intersection i2{2, s};
    Intersections xs;
    xs.insert(i1);
    xs.insert(i2);
    REQUIRE(!xs.empty());
    REQUIRE(xs.size() == 2);
    CHECK(xs[0].object == s);
    CHECK(xs[1].object == s);
    for (auto const i : xs) {
      CHECK(i.object == s);
    }
  }
  SUBCASE("Empty Intersections") {
    Intersections xs;
    CHECK(xs.empty());
    CHECK(std::begin(xs) == std::end(xs));
  }
}

TEST_CASE("Intersect sets the object on the intersection") {
  Ray r{Point{0, 0, -5}, Vec3{0, 0, 1}};
  Sphere s;
  auto xs = r.intersect(s);
  REQUIRE(xs.size() == 2);
  CHECK(xs[0].object == s);
  CHECK(xs[1].object == s);
}

TEST_CASE("The hit, when all intersections have positive t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{1, s};
  auto i2 = Intersection{2, s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i1);
}

TEST_CASE("The hit, when some intersections have negative t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{-1, s};
  auto i2 = Intersection{1, s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i2);
}

TEST_CASE("The hit, when all intersections have negative t") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{-2, s};
  auto i2 = Intersection{-1, s};
  xs.insert(i1);
  xs.insert(i2);
  auto i = xs.hit();
  REQUIRE(!i.has_value());
}

TEST_CASE("The hit is always the lowest nonnegative intersection") {
  Sphere s;
  Intersections xs;
  auto i1 = Intersection{5, s};
  auto i2 = Intersection{7, s};
  auto i3 = Intersection{-3, s};
  auto i4 = Intersection{2, s};
  xs.insert(i1);
  xs.insert(i2);
  xs.insert(i3);
  xs.insert(i4);
  auto i = xs.hit();
  REQUIRE(i.has_value());
  CHECK(*i == i4);
}

TEST_CASE("Translating a ray") {
  Ray r{Point{1, 2, 3}, Vec3{0, 1, 0}};
  auto r2 = r.transform(identity_matrix().translate(3, 4, 5));
  CHECK(r2.origin == Point{4, 6, 8});
  CHECK(r2.direction == Vec3{0, 1, 0});
}

TEST_CASE("Scaling a ray") {
  Ray r{Point{1, 2, 3}, Vec3{0, 1, 0}};
  auto r2 = r.transform(identity_matrix().scale(2, 3, 4));
  CHECK(r2.origin == Point{2, 6, 12});
  CHECK(r2.direction == Vec3{0, 3, 0});
}

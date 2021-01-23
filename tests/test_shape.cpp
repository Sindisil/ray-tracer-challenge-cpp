#include "doctest.h"

#include "shape.h"

#include <cmath>

#include "color.h"
#include "intersections.h"
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
using raytrace::Shape;
using raytrace::Vector3;

//////////////
// Shape tests

class TestShape : public Shape {
  using Shape::Shape;

private:
  Ray saved_ray_;

protected:
  auto local_normal_at(Point p) const -> Vector3 override {
    return Vector3{p.x, p.y, p.z};
  }
  void local_intersect(Ray r, Intersections &) override { saved_ray_ = r; }

public:
  Ray saved_ray() { return saved_ray_; }
};

TEST_CASE("A TestShape's default transformation") {
  TestShape s;
  REQUIRE(s.transform() == identity_matrix());
}

TEST_CASE("Creating a TestShape with a transformation") {
  auto t = identity_matrix().translated(2, 3, 4);
  TestShape s{t};
  REQUIRE(s.transform() == t);
}

TEST_CASE("Changing a TestShape's transformation") {
  TestShape s;
  auto t = identity_matrix().translated(2, 3, 4);
  s.transform(t);
  REQUIRE(s.transform() == t);
}

TEST_CASE("A TestShape has a default material") {
  auto s = TestShape{};
  CHECK(s.material() == Material());
}

TEST_CASE("A TestShape may be created with a specified material") {
  auto m = Material{Color{1.0f, 1.0f, 1.0f}};
  auto s = TestShape{m};
  CHECK(s.material() == m);
}

TEST_CASE("A TestShape may be assigned a material") {
  auto s = TestShape{};
  auto m = Material{};
  m.ambient(1.0f);
  s.material(m);
  CHECK(s.material() == m);
  m.ambient(.5f);
  CHECK(s.material() != m);
}

TEST_CASE("Intersecting a scaled shape with a ray") {
  auto s = TestShape{identity_matrix().scaled(2.0f, 2.0f, 2.0f)};
  auto r = Ray{Point{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto xs = s.intersect(r);
  CHECK(s.saved_ray().origin == Point{0.0f, 0.0f, -2.5f});
  CHECK(s.saved_ray().direction == Vector3{0.0f, 0.0f, 0.5f});
}

TEST_CASE("Intersecting a translated TestShape with a ray") {
  Ray r{Point{0, 0, -5}, Vector3{0, 0, 1}};
  TestShape s{identity_matrix().translated(5, 0, 0)};
  auto xs = s.intersect(r);
  CHECK(s.saved_ray().origin == Point{-5.0f, 0.0f, -5.0f});
  CHECK(s.saved_ray().direction == Vector3{0.0f, 0.0f, 1.0f});
}

TEST_CASE("Computing the normal on a translated TestShape") {
  TestShape s(identity_matrix().translated(0, 1, 0));
  auto n = s.normal_at(Point{0, 1.70711f, -0.70711f});
  CHECK(n == Vector3{0, 0.70711f, -0.70711f});
}

TEST_CASE("Computing the normal on a transformed TestShape") {
  TestShape s(identity_matrix().rotated_on_z(pi / 5.0f).scaled(1, 0.5f, 1));
  auto n = s.normal_at(Point{0, std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2});
  CHECK(n == Vector3{0, 0.97014f, -0.24254f});
}

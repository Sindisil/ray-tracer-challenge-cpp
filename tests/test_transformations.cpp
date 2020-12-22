#include "transformations.h"

#include "doctest.h"

#include "matrix.h"
#include "primitives.h"

using raytrace::identity_matrix;
using raytrace::Matrix;
using raytrace::Point;
using raytrace::Vec3;

TEST_CASE("A transformation matrix for the default orientation") {
  auto from = Point{0.0, 0.0, 0.0};
  auto to = Point{0.0, 0.0, -1.0};
  auto up = Vec3{0.0, 1.0, 0.0};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix());
}

TEST_CASE("A view transformation matrix looking in toward +z") {
  auto from = Point{0.0, 0.0, 0.0};
  auto to = Point{0.0, 0.0, 1.0};
  auto up = Vec3{0.0, 1.0, 0.0};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix().scale(-1.0, 1.0, -1.0));
}

TEST_CASE("The view transformation moves the world") {
  auto from = Point{0.0, 0.0, 8.0};
  auto to = Point{0.0, 0.0, 0.0};
  auto up = Vec3{0.0, 1.0, 0.0};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix().translate(0.0, 0.0, -8.0));
}

TEST_CASE("An arbitrary view transformation") {
  auto from = Point{1.0, 3.0, 2.0};
  auto to = Point{4.0, -2.0, 8.0};
  auto up = Vec3{1.0, 1.0, 0.0};
  auto t = view_transform(from, to, up);
  REQUIRE(t == Matrix<4>{{{-0.50709, 0.50709, 0.67612, -2.36643},
                          {0.76772, 0.60609, 0.12122, -2.82843},
                          {-0.35857, 0.59761, -0.71714, 0.00000},
                          {0.00000, 0.00000, 0.00000, 1.00000}}});
}
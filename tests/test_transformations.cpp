#include "transformations.h"

#include "doctest.h"

#include "matrix.h"
#include "primitives.h"

using raytrace::identity_matrix;
using raytrace::Matrix4;
using raytrace::Point;
using raytrace::Vector3;

TEST_CASE("A transformation matrix for the default orientation") {
  auto from = Point{0.0f, 0.0f, 0.0f};
  auto to = Point{0.0f, 0.0f, -1.0f};
  auto up = Vector3{0.0f, 1.0f, 0.0f};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix());
}

TEST_CASE("A view transformation matrix looking in toward +z") {
  auto from = Point{0.0f, 0.0f, 0.0f};
  auto to = Point{0.0f, 0.0f, 1.0f};
  auto up = Vector3{0.0f, 1.0f, 0.0f};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix().scaled(-1.0f, 1.0f, -1.0f));
}

TEST_CASE("The view transformation moves the world") {
  auto from = Point{0.0f, 0.0f, 8.0f};
  auto to = Point{0.0f, 0.0f, 0.0f};
  auto up = Vector3{0.0f, 1.0f, 0.0f};
  auto t = view_transform(from, to, up);
  REQUIRE(t == identity_matrix().translated(0.0f, 0.0f, -8.0f));
}

TEST_CASE("An arbitrary view transformation") {
  auto from = Point{1.0f, 3.0f, 2.0f};
  auto to = Point{4.0f, -2.0f, 8.0f};
  auto up = Vector3{1.0f, 1.0f, 0.0f};
  auto t = view_transform(from, to, up);
  REQUIRE(t == Matrix4{{-0.50709f, 0.50709f, 0.67612f, -2.36643f},
                       {0.76772f, 0.60609f, 0.12122f, -2.82843f},
                       {-0.35857f, 0.59761f, -0.71714f, 0.00000f},
                       {0.00000f, 0.00000f, 0.00000f, 1.00000f}});
}
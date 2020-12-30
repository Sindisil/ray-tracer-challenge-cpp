#include "camera.h"

#include "doctest.h"

#include "color.h"
#include "matrix.h"
#include "primitives.h"
#include "transformations.h"
#include "world.h"

#include <cmath>

using raytrace::Camera;
using raytrace::Color;
using raytrace::default_world;
using raytrace::identity_matrix;
using raytrace::pi;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Vector3;
using raytrace::view_transform;
using raytrace::World;

TEST_CASE("Constructing a camera") {
  auto c = Camera{160, 120, pi / 2};
  CHECK(c.h_size() == 160);
  CHECK(c.v_size() == 120);
  CHECK_EQ(c.fov(), doctest::Approx(pi / 2));
  CHECK(c.transform() == identity_matrix());
}

TEST_CASE("The pixel size for a horizontal canvas") {
  auto c = Camera{200, 125, pi / 2};
  CHECK_EQ(c.pixel_size(), doctest::Approx(0.01f));
}

TEST_CASE("The pixel size for a vertical canvas") {
  auto c = Camera{125, 200, pi / 2};
  CHECK_EQ(c.pixel_size(), doctest::Approx(0.01f));
}

TEST_CASE("Constructing rays through the points on the canvas") {
  auto c = Camera{201, 101, pi / 2};

  SUBCASE("Constructing a ray through the center of the canvas") {
    auto r = c.ray_for_pixel(100, 50);
    CHECK(r.origin == Point{0.0f, 0.0f, 0.0f});
    CHECK(r.direction == Vector3{0.0f, 0.0f, -1.0f});
  }

  SUBCASE("Constructing a ray through a corner of the canvas") {
    auto r = c.ray_for_pixel(0, 0);
    CHECK(r.origin == Point{0.0f, 0.0f, 0.0f});
    CHECK(r.direction == Vector3{0.66519f, 0.33259f, -0.66851f});
  }

  SUBCASE("Constructing a ray when the camera is transformed") {
    c.transform(
        identity_matrix().translate(0.0f, -2.0f, 5.0f).rotate_y(pi / 4));
    auto r = c.ray_for_pixel(100, 50);
    CHECK(r.origin == Point{0.0f, 2.0f, -5.0f});
    CHECK(r.direction ==
          Vector3{std::sqrt(2.0f) / 2, 0.0f, -std::sqrt(2.0f) / 2});
  }
}

TEST_CASE("Rendering a world with a camera") {
  auto w = default_world();
  auto c =
      Camera{11, 11, pi / 2,
             view_transform(Point{0.0f, 0.0f, -5.0f}, Point{0.0f, 0.0f, 0.0f},
                            Vector3{0.0f, 1.0f, 0.0f})};
  auto image = c.render(w);
  CHECK(image.pixel_at(5, 5) == Color{0.38066f, 0.47583f, 0.2855f});
}
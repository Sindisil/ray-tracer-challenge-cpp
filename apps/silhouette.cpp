#include <iostream>

#include "canvas.h"
#include "matrix.h"
#include "primitives.h"
#include "ray.h"
#include "sphere.h"

using raytrace::Canvas;
using raytrace::Color;
using raytrace::Coordinate;
using raytrace::identity_matrix;
using raytrace::intersect;
using raytrace::normalize;
using raytrace::pi;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Sphere;

int main() {
  auto const ray_origin = Point{0, 0, -5};
  constexpr float wall_z{10.0f};
  constexpr float wall_size{7.0f};
  constexpr int canvas_size{100};
  constexpr float half_wall{wall_size / 2};
  constexpr float pixel_size{wall_size / canvas_size};
  Color const red{1, 0, 0};

  Canvas canvas{canvas_size, canvas_size};
  Sphere shape;

  for (int y = 0; y < canvas_size; ++y) {
    float world_y = half_wall - pixel_size * y;

    for (int x = 0; x < canvas_size; ++x) {
      float world_x = half_wall - pixel_size * x;

      Point target{world_x, world_y, wall_z};

      Ray r{ray_origin, normalize(target - ray_origin)};
      auto xs = intersect(shape, r);
      if (xs.hit().has_value()) {
        canvas.write_pixel(x, y, red);
      }
    }
  }

  std::cout << canvas.to_ppm();
}
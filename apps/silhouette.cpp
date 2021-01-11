#include <chrono>
#include <iostream>

#include "canvas.h"
#include "matrix.h"
#include "primitives.h"
#include "ray.h"
#include "shapes.h"

using raytrace::Canvas;
using raytrace::Color;
using raytrace::Coordinate;
using raytrace::identity_matrix;
using raytrace::pi;
using raytrace::Point;
using raytrace::Ray;
using raytrace::Sphere;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

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

  auto begin = high_resolution_clock::now();

  for (int y = 0; y < canvas_size; ++y) {
    float world_y = half_wall - pixel_size * y;

    for (int x = 0; x < canvas_size; ++x) {
      float world_x = half_wall - pixel_size * x;

      Point target{world_x, world_y, wall_z};

      Ray r{ray_origin, (target - ray_origin).normalize()};
      auto xs = r.intersect(shape);
      if (xs.hit().has_value()) {
        canvas.write_pixel(x, y, red);
      }
    }
  }

  auto end_rendering = high_resolution_clock::now();

  auto ppm = canvas.to_ppm();

  auto end_create_ppm = high_resolution_clock::now();

  std::cout << ppm;

  auto end_write_ppm = high_resolution_clock::now();

  std::cerr << "\nRendering took "
            << duration_cast<milliseconds>(end_rendering - begin).count()
            << "ms.";
  std::cerr
      << "\nPPM generation took "
      << duration_cast<milliseconds>(end_create_ppm - end_rendering).count()
      << "ms.";
  std::cerr
      << "\nWriting PPM to stdout took "
      << duration_cast<milliseconds>(end_write_ppm - end_create_ppm).count()
      << "ms.\n";
}
#include <chrono>
#include <iostream>

#include "canvas.h"
#include "color.h"
#include "lights.h"
#include "materials.h"
#include "matrix.h"
#include "primitives.h"
#include "ray.h"
#include "sphere.h"

using raytrace::Canvas;
using raytrace::Color;
using raytrace::Coordinate;
using raytrace::identity_matrix;
using raytrace::lighting;
using raytrace::Material;
using raytrace::pi;
using raytrace::Point;
using raytrace::PointLight;
using raytrace::Ray;
using raytrace::Sphere;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main() {
  auto const ray_origin = Point{0, 0, -5};
  constexpr auto wall_z = 12.0f;
  constexpr auto wall_size = 10.0f;
  constexpr auto canvas_size = 400;
  constexpr auto half_wall = wall_size / 2.0f;
  constexpr auto pixel_size = wall_size / canvas_size;

  auto canvas = Canvas{canvas_size, canvas_size};
  auto shape = Sphere{Material{Color{1.0f, 1.0f, 1.0f}}};
  shape.material().shininess(50.0f);

  auto light =
      PointLight{Point{-10.0f, 10.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}};

  auto begin = high_resolution_clock::now();

  for (int y = 0; y < canvas_size; ++y) {
    auto world_y = half_wall - pixel_size * y;

    for (int x = 0; x < canvas_size; ++x) {
      auto world_x = -half_wall + pixel_size * x;

      auto target = Point{world_x, world_y, wall_z};

      auto r = Ray{ray_origin, (target - ray_origin).normalize()};
      auto xs = r.intersect(shape);
      if (xs.hit().has_value()) {
        auto hit = xs.hit().value();
        auto hit_point = r.position(hit.t);
        auto normal = hit.object.normal_at(hit_point);
        auto eye_dir = -r.direction;
        auto color =
            lighting(hit.object.material(), light, hit_point, eye_dir, normal);

        canvas.write_pixel(x, y, color);
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
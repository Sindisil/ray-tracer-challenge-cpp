#include "camera.h"
#include "color.h"
#include "materials.h"
#include "matrix.h"
#include "primitives.h"
#include "sphere.h"
#include "transformations.h"
#include "world.h"

#include <chrono>
#include <iostream>

using raytrace::Camera;
using raytrace::Color;
using raytrace::identity_matrix;
using raytrace::Material;
using raytrace::pi;
using raytrace::Point;
using raytrace::Sphere;
using raytrace::Vec3;
using raytrace::view_transform;
using raytrace::World;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main() {
  auto world = World{};

  auto floor = Sphere{};
  floor.transform(identity_matrix().scale(10.0, 0.01, 10.0));
  floor.material().specular(0.0).color(Color{1.0, 0.9, 0.9});
  world.push_back(floor);

  auto left_wall = Sphere{};
  left_wall.transform(identity_matrix()
                          .scale(10.0, 0.01, 10.0)
                          .rotate_x(pi / 2)
                          .rotate_y(-pi / 4)
                          .translate(0.0, 0.0, 5.0));
  left_wall.material(floor.material());
  world.push_back(left_wall);

  auto right_wall = Sphere{};
  right_wall.transform(identity_matrix()
                           .scale(10.0, 0.01, 10.0)
                           .rotate_x(pi / 2)
                           .rotate_y(pi / 4)
                           .translate(0.0, 0.0, 5.0));
  right_wall.material(floor.material());
  world.push_back(right_wall);

  auto middle = Sphere{};
  middle.transform(identity_matrix().translate(-0.5, 1.0, 0.5));
  middle.material().color(Color{0.1, 1.0, 0.5}).diffuse(0.7).specular(0.3);
  world.push_back(middle);

  auto right = Sphere{};
  right.transform(
      identity_matrix().scale(0.5, 0.5, 0.5).translate(1.5, 0.5, -0.5));
  right.material().color(Color{0.5, 1.0, 0.1}).diffuse(0.7).specular(0.3);
  world.push_back(right);

  auto left = Sphere{};
  left.transform(
      identity_matrix().scale(0.33, 0.33, 0.33).translate(-1.5, 0.33, -0.75));
  left.material().color(Color{1.0, 0.8, 0.1}).diffuse(0.7).specular(0.3);
  world.push_back(left);

  world.light().position = Point{-10.0, 10.0, -10.0};

  auto camera = Camera{800, 400, pi / 3};
  camera.transform(view_transform(Point{0.0, 1.5, -5.0}, Point{0.0, 1.0, 0.0},
                                  Vec3{0.0, 1.0, 0.0}));

  auto begin = high_resolution_clock::now();

  auto canvas = camera.render(world);

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
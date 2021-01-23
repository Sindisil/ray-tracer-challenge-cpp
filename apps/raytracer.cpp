#include "camera.h"
#include "color.h"
#include "materials.h"
#include "matrix.h"
#include "plane.h"
#include "primitives.h"
#include "shape.h"
#include "sphere.h"
#include "transformations.h"
#include "world.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

using raytrace::Camera;
using raytrace::Color;
using raytrace::identity_matrix;
using raytrace::Material;
using raytrace::pi;
using raytrace::Plane;
using raytrace::Point;
using raytrace::Sphere;
using raytrace::Vector3;
using raytrace::view_transform;
using raytrace::World;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

World define_scene() {
  auto world = World{};

  auto floor = Plane{};
  floor.material().specular(0.0f).color(Color{1.0f, 0.9f, 0.9f});
  world.push_back(std::move(std::make_unique<Plane>(floor)));

  auto left_wall = Plane{};
  left_wall.transform(
      identity_matrix().rotated_on_z(pi / 2).translated(-5.0f, 0.0f, 0.0f));
  left_wall.material(floor.material());
  world.push_back(std::move(std::make_unique<Plane>(left_wall)));

  auto right_wall = Plane{};
  right_wall.transform(
      identity_matrix().rotated_on_x(pi / 2).translated(0.0f, 0.0f, 10.0f));
  right_wall.material(floor.material());
  world.push_back(std::move(std::make_unique<Plane>(right_wall)));

  auto middle = Sphere{};
  middle.transform(identity_matrix().translated(-0.5f, 1.0f, 0.5f));
  middle.material().color(Color{0.1f, 1.0f, 0.5f}).diffuse(0.7f).specular(0.3f);
  world.push_back(std::move(std::make_unique<Sphere>(middle)));

  auto right = Sphere{};
  right.transform(
      identity_matrix().scaled(0.5f, 0.5f, 0.5f).translated(1.5f, 0.5f, -0.5f));
  right.material().color(Color{0.5f, 1.0f, 0.1f}).diffuse(0.7f).specular(0.3f);
  world.push_back(std::move(std::make_unique<Sphere>(right)));

  auto left = Sphere{};
  left.transform(identity_matrix()
                     .scaled(0.33f, 0.33f, 0.33f)
                     .translated(-1.5f, 0.33f, -0.75f));
  left.material().color(Color{1.0f, 0.8f, 0.1f}).diffuse(0.7f).specular(0.3f);
  world.push_back(std::move(std::make_unique<Sphere>(left)));

  world.light().position = Point{-1.0f, 2.0f, -3.0f};
  return world;
}

int main(int argc, char **argv) {
  int x_size = 200;
  int y_size = 100;

  if (argc > 1) {
    if (argc == 3) {
      x_size = std::stoi(std::string(argv[1]));
      y_size = std::stoi(std::string(argv[2]));
    }
  }
  auto world = define_scene();
  auto camera = Camera{x_size, y_size, pi / 3};
  camera.transform(view_transform(Point{0.0f, 1.5f, -5.0f},
                                  Point{0.0f, 1.0f, 0.0f},
                                  Vector3{0.0f, 1.0f, 0.0f}));

  auto begin = high_resolution_clock::now();

  auto canvas = camera.render(world);

  auto end_rendering = high_resolution_clock::now();

  auto ppm = canvas.to_ppm();

  auto end_create_ppm = high_resolution_clock::now();

  std::cout << ppm;

  auto end_write_ppm = high_resolution_clock::now();

  std::cerr << "\nImage " << x_size << " x " << y_size << "\n";
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
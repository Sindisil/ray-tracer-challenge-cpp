#include "materials.h"

#include "doctest.h"

#include "lights.h"
#include "primitives.h"

#include <cmath>

using raytrace::are_about_equal;
using raytrace::Color;
using raytrace::Material;
using raytrace::Point;
using raytrace::PointLight;
using raytrace::Vec3;

TEST_CASE("The default material") {
  auto m = Material{};
  CHECK(m.color() == Color{1, 1, 1});
  CHECK(are_about_equal(m.ambient(), 0.1));
  CHECK(are_about_equal(m.diffuse(), 0.9));
  CHECK(are_about_equal(m.specular(), 0.9));
  CHECK(are_about_equal(m.shininess(), 200.0));
}

TEST_CASE("Mutator functions throw on values < 0") {
  auto m = Material{};
  CHECK_THROWS_AS(m.ambient(-1.0), std::out_of_range);
  CHECK_THROWS_AS(m.diffuse(-1.0), std::out_of_range);
  CHECK_THROWS_AS(m.specular(-1.0), std::out_of_range);
  CHECK_THROWS_AS(m.shininess(-1.0), std::out_of_range);
}

TEST_CASE("Lighting function") {
  auto material = Material{};
  auto position = Point{0, 0, 0};

  SUBCASE("Lighting with the eye between the light and the surface") {
    auto eye = Vec3{0.0, 0.0, -1.0};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 0.0, -10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.9, 1.9, 1.9});
  }

  SUBCASE("Lighting with the eye between light and surface, eye offset 45°") {
    auto eye = Vec3{0.0, std::sqrt(2.0) / 2, -std::sqrt(2.0) / 2};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 0.0, -10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.0, 1.0, 1.0});
  }

  SUBCASE("Lighting with the eye opposite surface, light offset 45°") {
    auto eye = Vec3{0.0, 0.0, -1.0};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 10.0, -10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{0.7364, 0.7364, 0.7364});
  }

  SUBCASE("Lighting with the eye in the path of the reflection vector") {
    auto eye = Vec3{0.0, -std::sqrt(2.0) / 2, -std::sqrt(2.0) / 2};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 10.0, -10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.63639, 1.63639, 1.63639});
  }

  SUBCASE("Lighting with the eye between the light and the surface") {
    auto eye = Vec3{0.0, 0.0, -1.0};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 0.0, 10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{0.1, 0.1, 0.1});
  }

  SUBCASE("Lighting with the surface in shadow") {
    auto eye = Vec3{0.0, 0.0, -1.0};
    auto normal = Vec3{0.0, 0.0, -1.0};
    auto light = PointLight{Point{0.0, 0.0, 10.0}, Color{1.0, 1.0, 1.0}};
    auto color = lighting(material, light, position, eye, normal, true);
    CHECK(color == Color{0.1, 0.1, 0.1});
  }
}
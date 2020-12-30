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
using raytrace::Vector3;

TEST_CASE("The default material") {
  auto m = Material{};
  CHECK(m.color() == Color{1, 1, 1});
  CHECK(are_about_equal(m.ambient(), 0.1f));
  CHECK(are_about_equal(m.diffuse(), 0.9f));
  CHECK(are_about_equal(m.specular(), 0.9f));
  CHECK(are_about_equal(m.shininess(), 200.0f));
}

TEST_CASE("Mutator functions throw on values < 0") {
  auto m = Material{};
  CHECK_THROWS_AS(m.ambient(-1.0f), std::out_of_range);
  CHECK_THROWS_AS(m.diffuse(-1.0f), std::out_of_range);
  CHECK_THROWS_AS(m.specular(-1.0f), std::out_of_range);
  CHECK_THROWS_AS(m.shininess(-1.0f), std::out_of_range);
}

TEST_CASE("Lighting function") {
  auto material = Material{};
  auto position = Point{0, 0, 0};

  SUBCASE("Lighting with the eye between the light and the surface") {
    auto eye = Vector3{0.0f, 0.0f, -1.0f};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light = PointLight{Point{0.0f, 0.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.9f, 1.9f, 1.9f});
  }

  SUBCASE("Lighting with the eye between light and surface, eye offset 45°") {
    auto eye = Vector3{0.0f, std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light = PointLight{Point{0.0f, 0.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.0f, 1.0f, 1.0f});
  }

  SUBCASE("Lighting with the eye opposite surface, light offset 45°") {
    auto eye = Vector3{0.0f, 0.0f, -1.0f};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light =
        PointLight{Point{0.0f, 10.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{0.7364f, 0.7364f, 0.7364f});
  }

  SUBCASE("Lighting with the eye in the path of the reflection vector") {
    auto eye = Vector3{0.0f, -std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light =
        PointLight{Point{0.0f, 10.0f, -10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{1.63639f, 1.63639f, 1.63639f});
  }

  SUBCASE("Lighting with the eye between the light and the surface") {
    auto eye = Vector3{0.0f, 0.0f, -1.0f};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light = PointLight{Point{0.0f, 0.0f, 10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal);
    CHECK(color == Color{0.1f, 0.1f, 0.1f});
  }

  SUBCASE("Lighting with the surface in shadow") {
    auto eye = Vector3{0.0f, 0.0f, -1.0f};
    auto normal = Vector3{0.0f, 0.0f, -1.0f};
    auto light = PointLight{Point{0.0f, 0.0f, 10.0f}, Color{1.0f, 1.0f, 1.0f}};
    auto color = lighting(material, light, position, eye, normal, true);
    CHECK(color == Color{0.1f, 0.1f, 0.1f});
  }
}
#ifndef RAYTRACE_MATERIALS_H_GUARD
#define RAYTRACE_MATERIALS_H_GUARD

#include <stdexcept>

#include "color.h"
#include "lights.h"

namespace raytrace {

class Material {
public:
  Material() = default;
  Material(Color color) : color_(color){};

  auto color() -> Color const { return color_; }
  auto color(Color color) -> Material & {
    color_ = color;
    return *this;
  }

  auto ambient() -> double const { return ambient_; }
  auto ambient(double ambient) -> Material & {
    if (ambient >= 0) {
      ambient_ = ambient;
      return *this;
    } else {
      throw std::out_of_range("Ambient reflection must be non-negative");
    }
  }

  auto diffuse() -> double const { return diffuse_; }
  auto diffuse(double diffuse) -> Material & {
    if (diffuse >= 0) {
      diffuse_ = diffuse;
      return *this;
    } else {
      throw std::out_of_range("Diffuse reflection must be non-negative");
    }
  }

  auto specular() -> double const { return specular_; }
  auto specular(double specular) -> Material & {
    if (specular >= 0) {
      specular_ = specular;
      return *this;
    } else {
      throw std::out_of_range("Specular reflection must be non-negative");
    }
  }

  auto shininess() -> double const { return shininess_; }
  auto shininess(double shininess) -> Material & {
    if (shininess >= 0) {
      shininess_ = shininess;
      return *this;
    } else {
      throw std::out_of_range("Shininess must be non-negative");
    }
  }

  friend auto operator==(Material lhs, Material rhs) -> bool {
    return lhs.ambient() == rhs.ambient() && lhs.color() == rhs.color() &&
           lhs.diffuse() == rhs.diffuse() &&
           lhs.shininess() == rhs.shininess() &&
           lhs.specular() == rhs.specular();
  }

  friend auto operator!=(Material lhs, Material rhs) -> bool {
    return !(lhs == rhs);
  }

private:
  Color color_{1.0, 1.0, 1.0};
  double ambient_{0.1};
  double diffuse_{0.9};
  double specular_{0.9};
  double shininess_{200.0};
};

auto lighting(Material material, PointLight light, Point point, Vec3 eye,
              Vec3 normal, bool in_shadow = false) -> Color;

} // namespace raytrace
#endif

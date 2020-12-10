#ifndef RAYTRACE_MATERIALS_H
#define RAYTRACE_MATERIALS_H

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

  auto ambient() -> float const { return ambient_; }
  auto ambient(float ambient) -> Material & {
    if (ambient_ >= 0) {
      ambient_ = ambient;
      return *this;
    } else {
      throw std::out_of_range("Ambient reflection must be non-negative");
    }
  }

  auto diffuse() -> float const { return diffuse_; }
  auto diffuse(float diffuse) -> Material & {
    if (diffuse_ >= 0) {
      diffuse_ = diffuse;
      return *this;
    } else {
      throw std::out_of_range("Diffuse reflection must be non-negative");
    }
  }

  auto specular() -> float const { return specular_; }
  auto specular(float specular) -> Material & {
    if (specular_ >= 0) {
      specular_ = specular;
      return *this;
    } else {
      throw std::out_of_range("Specular reflection must be non-negative");
    }
  }

  auto shininess() -> float const { return shininess_; }
  auto shininess(float shininess) -> Material & {
    if (shininess_ >= 0) {
      shininess_ = shininess;
      return *this;
    } else {
      throw std::out_of_range("Shininess must be non-negative");
    }
  }

private:
  Color color_{1, 1, 1};
  float ambient_{0.1f};
  float diffuse_{0.9f};
  float specular_{0.9f};
  float shininess_{200.0f};
};

inline auto operator==(Material lhs, Material rhs) -> bool {
  return lhs.ambient() == rhs.ambient() && lhs.color() == rhs.color() &&
         lhs.diffuse() == rhs.diffuse() && lhs.shininess() == rhs.shininess() &&
         lhs.specular() == rhs.specular();
}

inline auto operator!=(Material lhs, Material rhs) -> bool {
  return !(lhs == rhs);
}

auto lighting(Material material, PointLight light, Point point, Vec3 eye,
              Vec3 normal) -> Color;

} // namespace raytrace
#endif

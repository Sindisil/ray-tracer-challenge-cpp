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

  auto ambient() const -> float { return ambient_; }
  auto ambient(float ambient) -> Material & {
    if (ambient >= 0) {
      ambient_ = ambient;
      return *this;
    } else {
      throw std::out_of_range("Ambient reflection must be non-negative");
    }
  }

  auto diffuse() const -> float { return diffuse_; }
  auto diffuse(float diffuse) -> Material & {
    if (diffuse >= 0) {
      diffuse_ = diffuse;
      return *this;
    } else {
      throw std::out_of_range("Diffuse reflection must be non-negative");
    }
  }

  auto specular() const -> float { return specular_; }
  auto specular(float specular) -> Material & {
    if (specular >= 0) {
      specular_ = specular;
      return *this;
    } else {
      throw std::out_of_range("Specular reflection must be non-negative");
    }
  }

  auto shininess() const -> float { return shininess_; }
  auto shininess(float shininess) -> Material & {
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
  Color color_{1.0f, 1.0f, 1.0f};
  float ambient_{0.1f};
  float diffuse_{0.9f};
  float specular_{0.9f};
  float shininess_{200.0f};
};

auto lighting(Material material, PointLight light, Point point, Vector3 eye,
              Vector3 normal, bool in_shadow = false) -> Color;

} // namespace raytrace
#endif

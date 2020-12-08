#ifndef RAYTRACE_MATERIALS_H
#define RAYTRACE_MATERIALS_H

#include <stdexcept>

#include "color.h"

namespace raytrace {

class Material {
public:
  Material() = default;
  Material(Color color) : color_(color){};

  auto color() -> Color const { return color_; }
  void color(Color color) { color_ = color; }

  auto ambient() -> float const { return ambient_; }
  void ambient(float ambient) {
    if (ambient_ >= 0) {
      ambient_ = ambient;
    } else {
      throw std::out_of_range("Ambient reflection must be non-negative");
    }
  }

  auto diffuse() -> float const { return diffuse_; }
  void diffuse(float diffuse) {
    if (diffuse_ >= 0) {
      diffuse_ = diffuse;
    } else {
      throw std::out_of_range("Diffuse reflection must be non-negative");
    }
  }

  auto specular() -> float const { return specular_; }
  void specular(float specular) {
    if (specular_ >= 0) {
      specular_ = specular;
    } else {
      throw std::out_of_range("Specular reflection must be non-negative");
    }
  }

  auto shininess() -> float const { return shininess_; }
  void shininess(float shininess) {
    if (shininess_ >= 0) {
      shininess_ = shininess;
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

} // namespace raytrace
#endif

#ifndef RAYTRACE_RAY_H_GUARD
#define RAYTRACE_RAY_H_GUARD

#include "matrix.h"
#include "primitives.h"

namespace raytrace {

struct Ray {
  Point origin{0, 0, 0};
  Vector3 direction{0, 0, 0};

  auto position(float t) const -> Point { return origin + direction * t; };
  auto transform(Matrix4 m) const -> Ray {
    return Ray{origin * m, direction * m};
  };

  friend auto operator==(Ray lhs, Ray rhs) -> bool {
    return lhs.origin == rhs.origin && lhs.direction == rhs.direction;
  }

  friend auto operator!=(Ray lhs, Ray rhs) -> bool { return !(lhs == rhs); }
};

} // namespace raytrace

#endif

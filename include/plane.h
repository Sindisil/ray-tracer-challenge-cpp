#ifndef RAYTRACE_PLANE_H_GUARD
#define RAYTRACE_PLANE_H_GUARD

#include "intersections.h"
#include "primitives.h"
#include "shape.h"

#include <cmath>

namespace raytrace {

class Plane : public Shape {
public:
  using Shape::Shape;

  auto local_normal_at(Point /* unused */) const -> Vector3 override {
    return Vector3{0.0f, 1.0f, 0.0f};
  }
  void local_intersect(Ray r, Intersections &xs) override {
    if (std::abs(r.direction.y) >= epsilon) {
      auto t = -r.origin.y / r.direction.y;
      xs.insert(Intersection{t, this});
    }
  }
};
} // namespace raytrace
#endif
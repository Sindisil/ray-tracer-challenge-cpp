#ifndef RAYTRACE_SPHERE_H_GUARD
#define RAYTRACE_SPHERE_H_GUARD

#include "shape.h"

#include <ostream>

namespace raytrace {

class Sphere : public Shape {
public:
  using Shape::Shape;

protected:
  auto local_normal_at(Point point) const -> Vector3 override;
  void local_intersect(Ray ray, Intersections &xs) override;

}; // namespace raytrace

auto operator<<(std::ostream &os, Sphere const &val) -> std::ostream &;

} // namespace raytrace
#endif
#include "sphere.h"

#include <ostream>

namespace raytrace {

auto Sphere::normal_at(Point world_point) const -> Vec3 {
  auto object_point = transform_.invert() * world_point;
  auto object_normal = object_point - Point{0, 0, 0};
  auto world_normal = transform_.invert().transpose() * object_normal;
  return world_normal.normalize();
}

auto operator<<(std::ostream &os, const Sphere &val) -> std::ostream & {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

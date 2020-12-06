#include "sphere.h"

#include <ostream>

namespace raytrace {

Vec3 Sphere::normal_at(Point world_point) {
  auto object_point = transform.invert() * world_point;
  auto object_normal = object_point - Point{0, 0, 0};
  auto world_normal = transform.invert().transpose() * object_normal;
  return world_normal.normalize();
}

std::ostream &operator<<(std::ostream &os, const Sphere &val) {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

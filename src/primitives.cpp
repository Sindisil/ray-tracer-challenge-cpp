#include "primitives.h"

#include <ostream>

namespace raytrace {

auto Vec3::reflect(Vec3 normal) const -> Vec3 {
  return *this - normal * 2 * this->dot(normal);
}

auto operator<<(std::ostream &os, Vec3 const &val) -> std::ostream & {
  os << "Vec3(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

auto operator<<(std::ostream &os, Point const &val) -> std::ostream & {
  os << "Point(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

} // namespace raytrace

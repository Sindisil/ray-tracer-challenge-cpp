#include "primitives.h"

#include <ostream>

namespace raytrace {

Vec3 Vec3::reflect(Vec3 normal) const {
  return *this - normal * 2 * this->dot(normal);
}

std::ostream &operator<<(std::ostream &os, Vec3 const &val) {
  os << "Vec3(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Point const &val) {
  os << "Point(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

} // namespace raytrace

#include "primitives.h"

#include <ostream>

namespace raytrace {

std::ostream &operator<<(std::ostream &os, Vec3 const &val) {
  os << "Vec3(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Point const &val) {
  os << "Point(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

bool operator==(Color c1, Color c2) {
  return aboutEqual(c1.r, c2.r) && aboutEqual(c1.r, c2.r) &&
         aboutEqual(c1.r, c2.r);
}

std::ostream &operator<<(std::ostream &os, Color const &val) {
  os << "Color(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

} // namespace raytrace
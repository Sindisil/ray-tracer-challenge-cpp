#include "primatives.h"

#include <iostream>

namespace raytrace {

std::ostream &operator<<(std::ostream &os, Vec3 const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Point const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

bool operator==(Color c1, Color c2) {
  return aboutEqual(c1.r, c2.r) && aboutEqual(c1.r, c2.r) &&
         aboutEqual(c1.r, c2.r);
}

std::ostream &operator<<(std::ostream &os, Color const &val) {
  os << "(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

} // namespace raytrace

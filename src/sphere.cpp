#include "sphere.h"

#include <ostream>

namespace raytrace {

std::ostream &operator<<(std::ostream &os, const Sphere &val) {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

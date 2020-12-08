#include "color.h"

#include "primitives.h"

#include <ostream>

namespace raytrace {

bool operator==(Color c1, Color c2) {
  return are_about_equal(c1.r, c2.r) && are_about_equal(c1.g, c2.g) &&
         are_about_equal(c1.b, c2.b);
}

std::ostream &operator<<(std::ostream &os, Color const &val) {
  os << "Color(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

} // namespace raytrace

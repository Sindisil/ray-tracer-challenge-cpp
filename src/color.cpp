#include "color.h"

#include "primitives.h"

#include <ostream>

namespace raytrace {

auto operator<<(std::ostream &os, Color const &val) -> std::ostream & {
  os << "Color(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

} // namespace raytrace

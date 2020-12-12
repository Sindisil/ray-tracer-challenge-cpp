#ifndef RAYTRACE_LIGHTS_H_GUARD
#define RAYTRACE_LIGHTS_H_GUARD

#include "color.h"
#include "primitives.h"

#include <ostream>

namespace raytrace {

struct PointLight {
  Point position{0.0f, 0.0f, 0.0f};
  Color intensity{colors::white};

  friend auto operator==(PointLight lhs, PointLight rhs) {
    return lhs.position == rhs.position && lhs.intensity == rhs.intensity;
  }

  friend auto operator!=(PointLight lhs, PointLight rhs) {
    return !(lhs == rhs);
  }
};

inline auto operator<<(std::ostream &os, PointLight const &val)
    -> std::ostream & {
  os << "PointLight(" << val.intensity << " @ " << val.position << ")";
  return os;
}

} // namespace raytrace
#endif

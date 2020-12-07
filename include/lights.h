#ifndef RAYTRACE_LIGHTS_H
#define RAYTRACE_LIGHTS_H

#include "primitives.h"

namespace raytrace {

struct PointLight {
  Point position;
  Color intensity;
};

} // namespace raytrace
#endif

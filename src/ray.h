#ifndef RAY_H_GUARD
#define RAY_H_GUARD

#include "primatives.h"

#include "doctest.h"

#include "primatives.h"

namespace raytrace {

struct Ray {
  Point origin{0, 0, 0};
  Vec3 direction{0, 0, 0};

  Point position(float t) { return origin + direction * t; };
};

} // namespace raytrace

#endif

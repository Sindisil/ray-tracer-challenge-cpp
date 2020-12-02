#ifndef RAY_H_GUARD
#define RAY_H_GUARD

#include "doctest.h"

#include <vector>

#include "primatives.h"
#include "sphere.h"

namespace raytrace {

struct Ray {
  Point origin{0, 0, 0};
  Vec3 direction{0, 0, 0};

  Point position(float t) { return origin + direction * t; };
};

std::vector<float> intersect(Sphere const &s, Ray const &r);

} // namespace raytrace

#endif

#ifndef RAY_H_GUARD
#define RAY_H_GUARD

#include "doctest.h"

#include <initializer_list>
#include <vector>

#include "primatives.h"
#include "sphere.h"

namespace raytrace {

struct Intersection {
  float t;
  Sphere object;
};

inline std::vector<Intersection>
intersections(std::initializer_list<Intersection> is) {
  std::vector<Intersection> v{is};
  return v;
}

struct Ray {
  Point origin{0, 0, 0};
  Vec3 direction{0, 0, 0};

  Point position(float t) { return origin + direction * t; };
};

std::vector<float> intersect(Sphere const &s, Ray const &r);

} // namespace raytrace

#endif

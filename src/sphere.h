#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include "primatives.h"

#include "doctest.h"

#include "primatives.h"

namespace raytrace {

struct Sphere {
  Point origin{0, 0, 0};
  float radius{1};
};

} // namespace raytrace

#endif

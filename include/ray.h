#ifndef RAY_H_GUARD
#define RAY_H_GUARD

#include <algorithm>
#include <initializer_list>
#include <ios>
#include <optional>
#include <vector>

#include "matrix.h"
#include "primitives.h"
#include "sphere.h"

namespace raytrace {

struct Intersection {
  float t;
  Sphere object;
};

inline bool operator<(Intersection lhs, Intersection rhs) {
  return lhs.t < rhs.t;
}

inline bool operator==(Intersection lhs, Intersection rhs) {
  return lhs.object == rhs.object && are_about_equal(lhs.t, rhs.t);
}

class Intersections {
public:
  using size_type = std::vector<Intersections>::size_type;

  Intersections &add(Intersection new_intersection) {
    auto i = std::upper_bound(intersections_.begin(), intersections_.end(),
                              new_intersection);
    intersections_.insert(i, new_intersection);
    return *this;
  }

  std::optional<Intersection> hit();

  Intersection operator[](size_type i) { return intersections_[i]; }

  size_type size() { return intersections_.size(); }

  bool empty() { return intersections_.empty(); }

private:
  std::vector<Intersection> intersections_;
};

struct Ray {
  Point origin{0, 0, 0};
  Vec3 direction{0, 0, 0};

  Point position(float t) const { return origin + direction * t; };
  Ray transform(Matrix<4> m) const { return Ray{origin * m, direction * m}; };
};

Intersections intersect(Sphere s, Ray r);

std::ostream &operator<<(std::ostream &os, Intersection const &val);

} // namespace raytrace

#endif

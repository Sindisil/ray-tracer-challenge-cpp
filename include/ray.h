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

  friend auto operator<(Intersection lhs, Intersection rhs) -> bool {
    return lhs.t < rhs.t;
  }

  friend auto operator==(Intersection lhs, Intersection rhs) -> bool {
    return lhs.object == rhs.object && are_about_equal(lhs.t, rhs.t);
  }
};

struct Ray {
  Point origin{0, 0, 0};
  Vec3 direction{0, 0, 0};

  auto position(float t) const -> Point { return origin + direction * t; };
  auto transform(Matrix<4> m) const -> Ray {
    return Ray{origin * m, direction * m};
  };

  friend auto operator==(Ray lhs, Ray rhs) -> bool {
    return lhs.origin == rhs.origin && lhs.direction == rhs.direction;
  }

  friend auto operator!=(Ray lhs, Ray rhs) -> bool { return !(lhs == rhs); }
};

auto operator<<(std::ostream &os, Intersection const &val) -> std::ostream &;

class Intersections {
public:
  using size_type = std::vector<Intersections>::size_type;

  auto add(Intersection new_intersection) -> Intersections & {
    auto i = std::upper_bound(intersections_.begin(), intersections_.end(),
                              new_intersection);
    intersections_.insert(i, new_intersection);
    return *this;
  }

  auto hit() const -> std::optional<Intersection>;

  auto operator[](size_type i) -> Intersection { return intersections_[i]; }

  auto size() const -> size_type { return intersections_.size(); }

  auto empty() const -> bool { return intersections_.empty(); }

private:
  std::vector<Intersection> intersections_;
};

auto intersect(Sphere s, Ray r) -> Intersections;

} // namespace raytrace

#endif

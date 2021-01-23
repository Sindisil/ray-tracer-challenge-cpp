#ifndef RAYTRACE_INTERSECTIONS_H_GUARD
#define RAYTRACE_INTERSECTIONS_H_GUARD

#include "primitives.h"
#include "shape.h"

#include <optional>
#include <ostream>
#include <vector>

namespace raytrace {

struct Intersection {
  float t;
  Shape *object;

  friend auto operator<(Intersection lhs, Intersection rhs) -> bool {
    return lhs.t < rhs.t;
  }

  friend auto operator==(Intersection lhs, Intersection rhs) -> bool {
    return *lhs.object == *rhs.object && are_about_equal(lhs.t, rhs.t);
  }
};

auto operator<<(std::ostream &os, Intersection const &val) -> std::ostream &;

class Intersections {
public:
  using value_type = Intersections;
  using size_type = std::vector<Intersections>::size_type;

  auto insert(Intersection new_intersection) -> Intersections & {
    auto i = std::upper_bound(intersections_.begin(), intersections_.end(),
                              new_intersection);
    intersections_.insert(i, new_intersection);
    return *this;
  }

  auto hit() const -> std::optional<Intersection>;

  auto operator[](size_type i) -> Intersection { return intersections_[i]; }

  auto size() const -> size_type { return intersections_.size(); }

  auto empty() const -> bool { return intersections_.empty(); }

  auto begin() -> std::vector<Intersection>::iterator {
    return intersections_.begin();
  }
  auto end() -> std::vector<Intersection>::iterator {
    return intersections_.end();
  }

private:
  std::vector<Intersection> intersections_;
};
} // namespace raytrace
#endif
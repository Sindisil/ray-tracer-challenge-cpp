#include "intersections.h"
#include "shape.h"

#include <ostream>

namespace raytrace {
auto Intersections::hit() const -> std::optional<Intersection> {
  auto h = std::find_if(intersections_.begin(), intersections_.end(),
                        [](Intersection const &i) { return i.t >= 0; });
  return h == intersections_.end() ? std::nullopt
                                   : std::optional<Intersection>(*h);
}

auto operator<<(std::ostream &os, Intersection const &val) -> std::ostream & {
  os << "Intersection(t=" << val.t << ", " << *val.object << ")";
  return os;
}
} // namespace raytrace
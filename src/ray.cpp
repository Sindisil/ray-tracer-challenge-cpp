#include "ray.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <ostream>
#include <vector>

#include "sphere.h"

namespace raytrace {

auto intersect(Sphere s, Ray r) -> Intersections {
  auto xs = Intersections{};
  // transform ray by inverse of sphere's transform,
  // instead of actually transforming the sphere
  r = r.transform(s.transform.invert());
  auto sphere_to_ray = r.origin - Point{0, 0, 0};
  auto a = dot(r.direction, r.direction);
  auto b = 2 * dot(r.direction, sphere_to_ray);
  auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
  auto discriminant = (b * b) - 4 * a * c;
  if (discriminant >= 0) {
    xs.add(Intersection{(-b - std::sqrt(discriminant)) / (2 * a), s});
    xs.add(Intersection{(-b + std::sqrt(discriminant)) / (2 * a), s});
  }
  return xs;
}

auto Intersections::hit() const -> std::optional<Intersection> {
  auto h = std::find_if(intersections_.begin(), intersections_.end(),
                        [](Intersection const &i) { return i.t >= 0; });
  return h == intersections_.end() ? std::nullopt
                                   : std::optional<Intersection>(*h);
}

auto operator<<(std::ostream &os, Intersection const &val) -> std::ostream & {
  os << "Intersection(t=" << val.t << ", " << val.object << ")";
  return os;
}

} // namespace raytrace
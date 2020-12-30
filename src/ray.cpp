#include "ray.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <ostream>
#include <vector>

#include "sphere.h"

namespace raytrace {

auto Ray::intersect(Sphere s) -> Intersections {
  auto xs = Intersections{};
  return intersect(s, xs);
}

auto Ray::intersect(Sphere s, Intersections &xs) -> Intersections {
  // transform ray by inverse of sphere's transform,
  // instead of actually transforming the sphere
  auto r = transform(s.transform().inverse());
  auto sphere_to_ray = r.origin - Point{0, 0, 0};
  auto a = r.direction.dot(r.direction);
  auto b = 2 * r.direction.dot(sphere_to_ray);
  auto c = sphere_to_ray.dot(sphere_to_ray) - 1;
  auto discriminant = (b * b) - 4 * a * c;
  if (discriminant >= 0) {
    xs.insert(Intersection{(-b - std::sqrt(discriminant)) / (2 * a), s});
    xs.insert(Intersection{(-b + std::sqrt(discriminant)) / (2 * a), s});
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
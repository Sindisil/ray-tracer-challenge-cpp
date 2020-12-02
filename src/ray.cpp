#include "ray.h"

#include <cmath>
#include <vector>

#include "sphere.h"

namespace raytrace {

std::vector<Intersection> intersect(Sphere const &s, Ray const &r) {
  std::vector<Intersection> xs;
  auto sphere_to_ray = r.origin - Point{0, 0, 0};
  auto a = dot(r.direction, r.direction);
  auto b = 2 * dot(r.direction, sphere_to_ray);
  auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
  auto discriminant = (b * b) - 4 * a * c;
  if (discriminant >= 0) {
    xs.push_back(Intersection{(-b - std::sqrt(discriminant)) / (2 * a), s});
    xs.push_back(Intersection{(-b + std::sqrt(discriminant)) / (2 * a), s});
  }
  return xs;
}

} // namespace raytrace
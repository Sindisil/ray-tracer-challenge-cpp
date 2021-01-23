#include "sphere.h"
#include "intersections.h"

namespace raytrace {

void Sphere::local_intersect(Ray ray, Intersections &xs) {
  auto sphere_to_ray = ray.origin - Point{0, 0, 0};
  auto a = ray.direction.dot(ray.direction);
  auto b = 2 * ray.direction.dot(sphere_to_ray);
  auto c = sphere_to_ray.dot(sphere_to_ray) - 1;
  auto discriminant = (b * b) - 4 * a * c;
  if (discriminant >= 0) {
    xs.insert(Intersection{(-b - std::sqrt(discriminant)) / (2 * a), this});
    xs.insert(Intersection{(-b + std::sqrt(discriminant)) / (2 * a), this});
  }
}

auto Sphere::local_normal_at(Point local_point) const -> Vector3 {
  return local_point - Point{0, 0, 0};
}

auto operator<<(std::ostream &os, const Sphere &val) -> std::ostream & {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}
} // namespace raytrace

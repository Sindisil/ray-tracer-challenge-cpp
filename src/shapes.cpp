#include "shapes.h"

#include "ray.h"

#include <ostream>

namespace raytrace {

auto Sphere::intersect(Ray ray, Intersections &xs) -> Intersections {
  // transform ray by inverse of sphere's transform,
  // instead of actually transforming the sphere
  auto r = ray.transform(transform().inverse());
  auto sphere_to_ray = r.origin - Point{0, 0, 0};
  auto a = r.direction.dot(r.direction);
  auto b = 2 * r.direction.dot(sphere_to_ray);
  auto c = sphere_to_ray.dot(sphere_to_ray) - 1;
  auto discriminant = (b * b) - 4 * a * c;
  if (discriminant >= 0) {
    xs.insert(Intersection{(-b - std::sqrt(discriminant)) / (2 * a), *this});
    xs.insert(Intersection{(-b + std::sqrt(discriminant)) / (2 * a), *this});
  }
  return xs;
}

auto Sphere::intersect(Ray r) -> Intersections {
  auto xs = Intersections{};
  return intersect(r, xs);
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

auto Sphere::normal_at(Point world_point) const -> Vector3 {
  auto object_point = transform_.inverse() * world_point;
  auto object_normal = object_point - Point{0, 0, 0};
  auto world_normal = transform_.inverse().transposed() * object_normal;
  return world_normal.normalize();
}

auto operator<<(std::ostream &os, const Sphere &val) -> std::ostream & {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

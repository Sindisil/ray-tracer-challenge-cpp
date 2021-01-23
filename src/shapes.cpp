#include "shapes.h"

#include "ray.h"

#include <ostream>

namespace raytrace {

auto Shape::normal_at(Point point) const -> Vector3 {
  auto local_normal = local_normal_at(transform_.inverse() * point);
  auto world_normal = transform_.inverse().transposed() * local_normal;
  return world_normal.normalize();
}

auto Shape::intersect(Ray ray, Intersections &xs) -> Intersections {
  auto local_ray = ray.transform(transform_.inverse());
  local_intersect(local_ray, xs);
  return xs;
}
auto Shape::intersect(Ray ray) -> Intersections {
  auto xs = Intersections{};
  return intersect(ray, xs);
}

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

auto Sphere::local_normal_at(Point local_point) const -> Vector3 {
  return local_point - Point{0, 0, 0};
}

auto operator<<(std::ostream &os, const Shape &val) -> std::ostream & {
  os << "Shape(Id: " << val.id() << ")";
  return os;
}
auto operator<<(std::ostream &os, const Sphere &val) -> std::ostream & {
  os << "Sphere(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

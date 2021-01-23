#include "shape.h"

#include "intersections.h"
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

auto operator<<(std::ostream &os, const Shape &val) -> std::ostream & {
  os << "Shape(Id: " << val.id() << ")";
  return os;
}

} // namespace raytrace

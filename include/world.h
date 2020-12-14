#ifndef RAYTRACE_WORLD_H_GUARD
#define RAYTRACE_WORLD_H_GUARD

#include "lights.h"
#include "primitives.h"
#include "ray.h"
#include "sphere.h"

#include <algorithm>
#include <vector>

namespace raytrace {

class World {
public:
  using value_type = Sphere;
  using size_type = std::vector<value_type>::size_type;

  auto contains(Sphere s) {
    return std::find(objects_.begin(), objects_.end(), s) != objects_.end();
  }

  auto push_back(Sphere s) -> World & {
    objects_.push_back(s);
    return *this;
  }

  auto size() const -> size_type { return objects_.size(); }

  auto empty() const -> bool { return objects_.empty(); }

  auto begin() -> std::vector<Sphere>::iterator { return objects_.begin(); }

  auto end() -> std::vector<Sphere>::iterator { return objects_.end(); }

  auto light() -> PointLight & { return light_; }
  auto light(PointLight light) { light_ = light; }

  auto intersect(Ray r) -> Intersections;

private:
  PointLight light_;
  std::vector<Sphere> objects_;
};

auto default_world() -> World;

struct PreComps {
  Intersection intersection;
  Point point;
  Vec3 eye_vec;
  Vec3 normal;

  PreComps(Intersection intersection, Ray ray) : intersection(intersection) {
    point = ray.position(intersection.t);
    eye_vec = -ray.direction;
    normal = intersection.object.normal_at(point);
  }
};

} // namespace raytrace
#endif
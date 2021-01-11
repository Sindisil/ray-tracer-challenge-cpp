#ifndef RAYTRACE_WORLD_H_GUARD
#define RAYTRACE_WORLD_H_GUARD

#include "lights.h"
#include "primitives.h"
#include "ray.h"
#include "shapes.h"

#include <algorithm>
#include <vector>

namespace raytrace {

class PreComps {
public:
  PreComps(Intersection intersection, Ray ray) : intersection_(intersection) {
    point_ = ray.position(intersection.t);
    eye_vec_ = -ray.direction;
    normal_ = intersection.object.normal_at(point_);
    if (normal_.dot(eye_vec_) < 0) {
      inside_ = true;
      normal_ = -normal_;
    } else {
      inside_ = false;
    }
    over_point_ = point_ + normal_ * bias;
  }

  auto intersection() const -> Intersection { return intersection_; }
  auto point() const -> Point { return point_; }
  auto over_point() const -> Point { return over_point_; }
  auto eye_vec() const -> Vector3 { return eye_vec_; }
  auto normal() const -> Vector3 { return normal_; }
  auto inside() const -> bool { return inside_; }

private:
  Intersection intersection_;
  Point point_;
  Point over_point_;
  Vector3 eye_vec_;
  Vector3 normal_;
  bool inside_;

  constexpr static float bias = epsilon * 50;
};

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

  auto operator[](size_type i) -> value_type { return objects_[i]; }

  auto light() -> PointLight & { return light_; }
  auto light(PointLight light) -> World & {
    light_ = light;
    return *this;
  }

  auto intersect(Ray r) const -> Intersections;

  auto shade_hit(PreComps comps) const -> Color;

  auto color_at(Ray r) const -> Color;

  auto is_shadowed(Point p) const -> bool;

private:
  PointLight light_;
  std::vector<Sphere> objects_;
};

auto default_world() -> World;

} // namespace raytrace
#endif

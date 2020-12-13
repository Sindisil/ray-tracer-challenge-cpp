#ifndef RAYTRACE_WORLD_H_GUARD
#define RAYTRACE_WORLD_H_GUARD

#include "lights.h"
#include "primitives.h"
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

private:
  PointLight light_;
  std::vector<Sphere> objects_;
};

// Create World containing:
// * 1 white point light at -10, 10, -10
// * two concentric spheres:
//    1 with Color{0.8, 1.0, .6}, diffuse{0.7}, and specular{0.2}
//    1 default material, but scaled by 0.5 in each dimension
auto default_world() -> World {
  auto w = World{};
  w.light(PointLight{Point{-10.0f, 10.0f, -10.0f}, colors::white});

  auto m = Material{Color{0.8f, 1.0f, 0.6f}}.diffuse(0.7f).specular(0.2f);
  w.push_back(Sphere{m});

  w.push_back(Sphere{identity_matrix().scale(0.5f, 0.5f, 0.5f)});
  return w;
}

} // namespace raytrace
#endif
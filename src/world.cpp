#include "world.h"

#include "ray.h"

namespace raytrace {

auto World::intersect(Ray r) -> Intersections {
  auto xs = Intersections{};
  for (auto s : objects_) {
    r.intersect(s, xs);
  }
  return xs;
}

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

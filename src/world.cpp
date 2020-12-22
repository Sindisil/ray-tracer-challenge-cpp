#include "world.h"

#include "ray.h"

#include <numeric>

namespace raytrace {

auto World::intersect(Ray r) const -> Intersections {
  return std::accumulate(
      objects_.begin(), objects_.end(), Intersections{},
      [&r](auto xs, auto obj) { return r.intersect(obj, xs); });
}

auto World::shade_hit(PreComps comps) const -> Color {
  return lighting(comps.intersection.object.material(), light_, comps.point,
                  comps.eye_vec, comps.normal, is_shadowed(comps.over_point));
}

auto World::color_at(Ray r) const -> Color {
  auto xs = intersect(r);
  auto h = xs.hit();
  return h ? shade_hit(PreComps{*h, r}) : colors::black;
}

auto World::is_shadowed(Point p) const -> bool {
  auto v = light_.position - p;
  auto distance = v.magnitude();
  auto direction = v.normalize();
  auto xs = intersect(Ray{p, direction});
  auto h = xs.hit();
  return h.has_value() && h->t < distance;
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

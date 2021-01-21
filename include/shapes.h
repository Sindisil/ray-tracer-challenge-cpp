#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include <atomic>
#include <cstdint>
#include <optional>
#include <ostream>
#include <vector>

#include "materials.h"
#include "matrix.h"
#include "primitives.h"
#include "ray.h"

namespace raytrace {

class Intersections;

static std::atomic<unsigned> next_id{1};

class Sphere {
public:
  Sphere(Material material, Matrix4 transform)
      : material_(material), transform_(transform) {
    id_ = std::atomic_fetch_add(&next_id, 1);
  }
  Sphere(Material material) : Sphere{material, identity_matrix()} {}
  Sphere(Matrix4 transform) : Sphere{Material{}, transform} {}
  Sphere() : Sphere{Material{}, identity_matrix()} {}

  auto transform() -> Matrix4 & { return transform_; }
  Sphere &transform(Matrix4 transform) {
    transform_ = transform;
    return *this;
  }

  auto material() -> Material & { return material_; }
  Sphere &material(Material material) {
    material_ = material;
    return *this;
  }

  auto id() const -> unsigned { return id_; }
  auto is(Sphere const &s) const -> bool { return s.id() == id_; }

  auto normal_at(Point p) const -> Vector3;

  auto intersect(Ray r, Intersections &xs) -> Intersections;
  auto intersect(Ray r) -> Intersections;

  friend auto operator==(Sphere const &lhs, Sphere const &rhs) -> bool {
    return lhs.transform_ == rhs.transform_ && lhs.material_ == rhs.material_;
  }

  friend auto operator!=(Sphere const &lhs, Sphere const &rhs) -> bool {
    return !(lhs == rhs);
  }

private:
  Material material_;
  Matrix4 transform_;
  unsigned id_;
};

auto operator<<(std::ostream &os, Sphere const &val) -> std::ostream &;

struct Intersection {
  float t;
  Sphere *object;

  friend auto operator<(Intersection lhs, Intersection rhs) -> bool {
    return lhs.t < rhs.t;
  }

  friend auto operator==(Intersection lhs, Intersection rhs) -> bool {
    return *lhs.object == *rhs.object && are_about_equal(lhs.t, rhs.t);
  }
};

auto operator<<(std::ostream &os, Intersection const &val) -> std::ostream &;

class Intersections {
public:
  using value_type = Intersections;
  using size_type = std::vector<Intersections>::size_type;

  auto insert(Intersection new_intersection) -> Intersections & {
    auto i = std::upper_bound(intersections_.begin(), intersections_.end(),
                              new_intersection);
    intersections_.insert(i, new_intersection);
    return *this;
  }

  auto hit() const -> std::optional<Intersection>;

  auto operator[](size_type i) -> Intersection { return intersections_[i]; }

  auto size() const -> size_type { return intersections_.size(); }

  auto empty() const -> bool { return intersections_.empty(); }

  auto begin() -> std::vector<Intersection>::iterator {
    return intersections_.begin();
  }
  auto end() -> std::vector<Intersection>::iterator {
    return intersections_.end();
  }

private:
  std::vector<Intersection> intersections_;
};

} // namespace raytrace

#endif

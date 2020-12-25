#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include <atomic>
#include <cstdint>

#include "materials.h"
#include "matrix.h"
#include "primitives.h"

namespace raytrace {

static std::atomic<unsigned> next_id{1};

class Sphere {
public:
  Sphere(Material material, Mat4 transform)
      : material_(material), transform_(transform) {
    id_ = std::atomic_fetch_add(&next_id, 1);
  }
  Sphere(Material material) : Sphere{material, identity_matrix()} {}
  Sphere(Mat4 transform) : Sphere{Material{}, transform} {}
  Sphere() : Sphere{Material{}, identity_matrix()} {}

  auto transform() -> Mat4 & { return transform_; }
  Sphere &transform(Mat4 transform) {
    transform_ = transform;
    return *this;
  }

  auto material() -> Material & { return material_; }
  Sphere &material(Material material) {
    material_ = material;
    return *this;
  }

  auto id() const -> unsigned { return id_; }
  auto is(Sphere s) const -> bool { return s.id() == id_; }

  auto normal_at(Point p) const -> Vec3;

  friend auto operator==(Sphere lhs, Sphere rhs) -> bool {
    return lhs.transform_ == rhs.transform_ && lhs.material_ == rhs.material_;
  }

  friend auto operator!=(Sphere lhs, Sphere rhs) -> bool {
    return !(lhs == rhs);
  }

private:
  Mat4 transform_;
  Material material_;
  unsigned id_;
};

auto operator<<(std::ostream &os, Sphere const &val) -> std::ostream &;

} // namespace raytrace

#endif

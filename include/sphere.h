#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include <atomic>
#include <cstdint>

#include "materials.h"
#include "matrix.h"
#include "primitives.h"

namespace raytrace {

static std::atomic<unsigned> next_id(0);

class Sphere {
public:
  Matrix<4> transform{identity_matrix()};
  Material material{};

  Sphere() { id_ = std::atomic_fetch_add(&next_id, 1); }
  Sphere(Matrix<4> transform) : transform(transform) { Sphere(); };
  Sphere(Material material) : material(material) { Sphere(); }

  auto id() const -> unsigned { return id_; }
  auto normal_at(Point p) const -> Vec3;

  friend auto operator==(Sphere lhs, Sphere rhs) -> bool {
    return lhs.id() == rhs.id();
  }
  friend auto operator!=(Sphere lhs, Sphere rhs) -> bool {
    return !(lhs == rhs);
  }

private:
  unsigned id_;
};

auto operator<<(std::ostream &os, Sphere const &val) -> std::ostream &;

} // namespace raytrace

#endif

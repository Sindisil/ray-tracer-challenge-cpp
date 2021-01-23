#ifndef RAYTRACE_SHAPE_H_GUARD
#define RAYTRACE_SHAPE_H_GUARD

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

class Shape {
private:
  Material material_;
  Matrix4 transform_;
  unsigned id_;

public:
  Shape(Material material, Matrix4 transform)
      : material_(material), transform_(transform) {
    id_ = std::atomic_fetch_add(&next_id, 1);
  }
  Shape(Material material) : Shape{material, identity_matrix()} {}
  Shape(Matrix4 transform) : Shape{Material{}, transform} {}
  Shape() : Shape{Material{}, identity_matrix()} {}
  virtual ~Shape() = default;

  virtual auto local_normal_at(Point p) const -> Vector3 = 0;
  virtual void local_intersect(Ray r, Intersections &xs) = 0;

  auto transform() -> Matrix4 & { return transform_; }
  void transform(Matrix4 transform) { transform_ = transform; }

  auto material() -> Material & { return material_; }
  void material(Material material) { material_ = material; }

  auto id() const -> unsigned { return id_; }
  auto is(Shape const &s) const -> bool { return s.id() == id_; }

  auto normal_at(Point point) const -> Vector3;

  auto intersect(Ray r, Intersections &xs) -> Intersections;
  auto intersect(Ray ray) -> Intersections;

  friend auto operator==(Shape const &lhs, Shape const &rhs) -> bool {
    return lhs.transform_ == rhs.transform_ && lhs.material_ == rhs.material_;
  }

  friend auto operator!=(Shape const &lhs, Shape const &rhs) -> bool {
    return !(lhs == rhs);
  }
};

auto operator<<(std::ostream &os, const Shape &val) -> std::ostream &;

} // namespace raytrace

#endif

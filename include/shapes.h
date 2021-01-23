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

class Shape {
private:
  Material material_;
  Matrix4 transform_;
  unsigned id_;

protected:
  virtual auto local_normal_at(Point p) const -> Vector3 = 0;
  virtual void local_intersect(Ray r, Intersections &xs) = 0;

public:
  Shape(Material material, Matrix4 transform)
      : material_(material), transform_(transform) {
    id_ = std::atomic_fetch_add(&next_id, 1);
  }
  Shape(Material material) : Shape{material, identity_matrix()} {}
  Shape(Matrix4 transform) : Shape{Material{}, transform} {}
  Shape() : Shape{Material{}, identity_matrix()} {}
  virtual ~Shape() = default;

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

class Sphere : public Shape {
public:
  using Shape::Shape;

protected:
  auto local_normal_at(Point point) const -> Vector3 override;
  void local_intersect(Ray ray, Intersections &xs) override;

}; // namespace raytrace

auto operator<<(std::ostream &os, const Shape &val) -> std::ostream &;
auto operator<<(std::ostream &os, Sphere const &val) -> std::ostream &;

struct Intersection {
  float t;
  Shape *object;

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

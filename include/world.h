#ifndef RAYTRACE_WORLD_H_GUARD
#define RAYTRACE_WORLD_H_GUARD

#include "intersections.h"
#include "lights.h"
#include "primitives.h"
#include "ray.h"

#include "shape.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace raytrace {

class PreComps {
public:
  PreComps(Intersection intersection, Ray ray) : intersection_(intersection) {
    point_ = ray.position(intersection.t);
    eye_vec_ = -ray.direction;
    normal_ = intersection.object->normal_at(point_);
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
private:
  PointLight light_;
  std::vector<std::unique_ptr<Shape>> objects_;

public:
  using shape_container = decltype(objects_);
  using difference_type = shape_container::difference_type;
  using value_type = Shape;
  using pointer = value_type *;
  using reference = value_type &;
  using size_type = shape_container::size_type;

  World() = default;
  World(World &&other) = default;

  class ShapeIterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = World::difference_type;
    using value_type = World::value_type;
    using pointer = World::pointer;
    using reference = World::reference;
    using size_type = World::size_type;

    ShapeIterator(shape_container::iterator iter) : iter_(iter) {}

    auto operator*() -> reference { return **iter_; }
    auto operator->() -> pointer { return iter_->get(); }
    auto operator[](size_type pos) -> reference { return *iter_[pos]; }

    auto operator++() -> ShapeIterator & {
      ++iter_;
      return *this;
    }

    auto operator++(int) -> ShapeIterator & {
      ShapeIterator &i = *this;
      ++(*this);
      return i;
    }

    auto operator--() -> ShapeIterator & {
      --iter_;
      return *this;
    }

    auto operator--(int) -> ShapeIterator & {
      ShapeIterator &i = *this;
      ++(*this);
      return i;
    }

    auto operator-=(difference_type n) -> ShapeIterator & {
      iter_ -= n;
      return *this;
    }

    auto operator+=(difference_type n) -> ShapeIterator & {
      iter_ += n;
      return *this;
    }

    auto operator-(ShapeIterator const &other) -> difference_type {
      return iter_ - other.iter_;
    }

    friend auto operator==(ShapeIterator const &lhs, ShapeIterator const &rhs)
        -> bool {
      return lhs.iter_ == rhs.iter_;
    }
    friend auto operator!=(ShapeIterator const &lhs, ShapeIterator const &rhs)
        -> bool {
      return lhs.iter_ != rhs.iter_;
    }
    friend auto operator<(ShapeIterator const &lhs, ShapeIterator const &rhs)
        -> bool {
      return lhs.iter_ < rhs.iter_;
    }

  private:
    shape_container::iterator iter_;
  };

  auto contains(Shape const &s) {
    return std::find_if(begin(), end(),
                        [&s](Shape const &obj) { return obj == s; }) != end();
  }

  auto push_back(std::unique_ptr<Shape> s) -> World & {
    objects_.push_back(std::move(s));
    return *this;
  }

  auto size() const -> size_type { return objects_.size(); }

  auto empty() const -> bool { return objects_.empty(); }

  auto begin() -> ShapeIterator { return ShapeIterator(objects_.begin()); }

  auto end() -> ShapeIterator { return ShapeIterator(objects_.end()); }

  auto operator[](size_type i) -> reference { return *objects_[i]; }

  auto light() -> PointLight & { return light_; }
  auto light(PointLight light) -> World & {
    light_ = light;
    return *this;
  }

  auto intersect(Ray r) const -> Intersections;

  auto shade_hit(PreComps comps) const -> Color;

  auto color_at(Ray r) const -> Color;

  auto is_shadowed(Point p) const -> bool;
};

inline auto operator-(World::ShapeIterator iter,
                      World::ShapeIterator::difference_type n)
    -> World::ShapeIterator {
  return iter -= n;
}

inline auto operator+(World::ShapeIterator iter,
                      World::ShapeIterator::difference_type n)
    -> World::ShapeIterator {
  return iter += n;
}

auto default_world() -> World;

} // namespace raytrace
#endif

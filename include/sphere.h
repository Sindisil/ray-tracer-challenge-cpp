#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include <atomic>
#include <cstdint>

#include "matrix.h"
#include "primitives.h"

namespace raytrace {

static std::atomic<unsigned> next_id(0);

class Sphere {
public:
  Matrix<4> transform{identity_matrix()};

  Sphere() { id_ = std::atomic_fetch_add(&next_id, 1); }
  Sphere(Matrix<4> transform) : transform(transform) { Sphere(); };
  unsigned id() const { return id_; }
  Vec3 normal_at(Point p);

private:
  unsigned id_;
};

inline bool operator==(Sphere lhs, Sphere rhs) { return lhs.id() == rhs.id(); }

std::ostream &operator<<(std::ostream &os, Sphere const &val);

} // namespace raytrace

#endif

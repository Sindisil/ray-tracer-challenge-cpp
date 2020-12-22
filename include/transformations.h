#ifndef RAYTRACE_TRANSFORMATIONS_H_GUARD
#define RAYTRACE_TRANSFORMATIONS_H_GUARD

#include "matrix.h"
#include "primitives.h"

namespace raytrace {

inline auto view_transform(Point from, Point to, Vec3 up) -> Matrix<4> {
  auto forward = (to - from).normalize();
  auto left = cross(forward, up.normalize());
  auto true_up = cross(left, forward);
  auto orientation = Matrix<4>{{{left.x, left.y, left.z, 0.0},
                                {true_up.x, true_up.y, true_up.z, 0.0},
                                {-forward.x, -forward.y, -forward.z, 0.0},
                                {0.0, 0.0, 0.0, 1.0}}};
  return orientation * identity_matrix().translate(-from.x, -from.y, -from.z);
}
} // namespace raytrace
#endif
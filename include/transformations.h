#ifndef RAYTRACE_TRANSFORMATIONS_H_GUARD
#define RAYTRACE_TRANSFORMATIONS_H_GUARD

#include "matrix.h"
#include "primitives.h"

namespace raytrace {

inline auto view_transform(Point from, Point to, Vector3 up) -> Matrix4 {
  auto forward = (to - from).normalize();
  auto left = forward.cross(up.normalize());
  auto true_up = left.cross(forward);
  auto orientation = Matrix4{{left.x, left.y, left.z, 0.0f},
                             {true_up.x, true_up.y, true_up.z, 0.0f},
                             {-forward.x, -forward.y, -forward.z, 0.0f},
                             {0.0f, 0.0f, 0.0f, 1.0f}};
  return orientation * identity_matrix().translated(-from.x, -from.y, -from.z);
}
} // namespace raytrace
#endif
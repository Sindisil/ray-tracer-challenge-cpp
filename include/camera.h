#ifndef RAYTRACE_CAMERA_H_GUARD
#define RAYTRACE_CAMERA_H_GUARD

#include "matrix.h"
#include "ray.h"

#include <cmath>

namespace raytrace {

class Camera {
public:
  Camera(int h_size, int v_size, float fov,
         Matrix<4> transform = identity_matrix())
      : h_size_(h_size), v_size_(v_size), fov_(fov), transform_(transform) {
    compute_pixel_size();
  }

  auto h_size() const -> int { return h_size_; }
  auto v_size() const -> int { return v_size_; }
  auto fov() const -> float { return fov_; }
  auto transform() const -> Matrix<4> { return transform_; }
  auto transform(Matrix<4> transform) -> Camera & {
    transform_ = transform;
    return *this;
  }

  auto pixel_size() const -> float { return pixel_size_; }

  auto ray_for_pixel(int x, int y) const -> Ray;

private:
  int h_size_;
  int v_size_;
  float fov_;
  Matrix<4> transform_;
  float half_width_;
  float half_height_;
  float pixel_size_;

  void compute_pixel_size();
};

} // namespace raytrace

#endif

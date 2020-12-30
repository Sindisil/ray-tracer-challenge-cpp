#include "camera.h"

#include "canvas.h"
#include "primitives.h"

namespace raytrace {

void Camera::compute_pixel_size() {
  auto half_view = std::tan(fov_ / 2);
  auto aspect = h_size_ / (v_size_ * 1.0f);
  if (aspect >= 1) {
    half_width_ = half_view;
    half_height_ = half_view / aspect;
  } else {
    half_width_ = half_view * aspect;
    half_height_ = half_view;
  }
  pixel_size_ = (half_width_ * 2) / h_size_;
}

auto Camera::ray_for_pixel(int x, int y) const -> Ray {
  // offset from the edge of the canvas to pixel's center
  auto x_offset = (x + 0.5f) * pixel_size_;
  auto y_offset = (y + 0.5f) * pixel_size_;

  // untransformed world space coordinates of the pixel
  auto world_x = half_width_ - x_offset;
  auto world_y = half_height_ - y_offset;
  auto inverse_transform = transform_.inverse();
  auto pixel = inverse_transform * Point{world_x, world_y, -1.0f};
  auto origin = inverse_transform * Point{0.0f, 0.0f, 0.0f};
  auto direction = (pixel - origin).normalize();

  return Ray{origin, direction};
}

auto Camera::render(World world) const -> Canvas {
  auto image = Canvas{h_size_, v_size_};

  for (int y = 0; y < v_size_; ++y) {
    for (int x = 0; x < h_size_; ++x) {
      auto ray = ray_for_pixel(x, y);
      image.write_pixel(x, y, world.color_at(ray));
    }
  }

  return image;
}

} // namespace raytrace

#ifndef CANVAS_H
#define CANVAS_H

#include "primitives.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace raytrace {

struct Coordinate {
  int x;
  int y;
};

class Canvas {
public:
  Canvas(int width, int height) : m_width(width), m_height(height) {
    if (width <= 0 || height <= 0) {
      throw std::out_of_range("height and width must be greater than zero");
    }
    m_pixels = std::vector<Color>(static_cast<size_t>(width) *
                                      static_cast<size_t>(height),
                                  Color{0, 0, 0});
  }

  Color pixel_at(int x, int y) {
    if (x < 0 || y < 0) {
      throw std::out_of_range("Pixel coordinates must be non-negative");
    }
    return m_pixels.at(x + static_cast<size_t>(y) * m_width);
  }

  void write_pixel(int x, int y, Color c) {
    if (x < 0 || y < 0) {
      throw std::out_of_range("Pixel coordinates must be non-negative");
    }
    m_pixels.at(x + static_cast<size_t>(y) * m_width) = c;
  }

  int width() { return m_width; }
  int height() { return m_height; }

  std::string to_ppm();

private:
  int m_width;
  int m_height;
  std::vector<Color> m_pixels;
};
} // namespace raytrace
#endif // ! CANVAS_H
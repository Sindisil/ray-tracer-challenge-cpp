#ifndef CANVAS_H
#define CANVAS_H

#include "primatives.h"

#include <stdexcept>
#include <vector>

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
    return m_pixels.at(x + y * m_width);
  }

  void write_pixel(int x, int y, Color c) {
    if (x < 0 || y < 0) {
      throw std::out_of_range("Pixel coordinates must be non-negative");
    }
    m_pixels.at(x + y * m_width) = c;
  }

  int width() { return m_width; }
  int height() { return m_height; }

private:
  int m_width;
  int m_height;
  std::vector<Color> m_pixels;

};
#endif // ! CANVAS_H
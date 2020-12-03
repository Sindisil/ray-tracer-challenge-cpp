#include "canvas.h"
#include "matrix.h"
#include "primitives.h"

#include <algorithm>
#include <iostream>

using raytrace::Canvas;
using raytrace::Color;
using raytrace::identityMatrix;
using raytrace::Matrix;
using raytrace::pi;
using raytrace::Point;

struct coordinate {
  int x;
  int y;
};

int main() {
  Canvas canvas{800, 800};
  constexpr coordinate center{400, 400};
  constexpr int radius{300};

  Point p{0, 1, 0}; // start at noon
  for (int i = 0; i < 12; ++i) {
    canvas.write_pixel(
        canvas.width() - center.x + static_cast<int>(p.x * radius),
        canvas.height() - center.y + static_cast<int>(p.y * radius),
        Color{240, 0, 0});
    p = identityMatrix().rotate_z(pi / 6) * p;
  }

  std::cout << canvas.to_ppm();
}
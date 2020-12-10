#ifndef RAYTRACE_COLOR_H
#define RAYTRACE_COLOR_H

#include <ostream>

namespace raytrace {

struct Color {
  float r;
  float g;
  float b;

  auto operator+=(Color c) -> Color & {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
  }

  auto operator-=(Color c) -> Color & {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
  }

  auto operator*=(float f) -> Color & {
    r *= f;
    g *= f;
    b *= f;
    return *this;
  }

  auto operator*=(Color c) -> Color & {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
  }
};

auto operator<<(std::ostream &os, Color const &val) -> std::ostream &;

auto operator==(Color c1, Color c2) -> bool;
inline auto operator!=(Color c1, Color c2) -> bool { return !(c1 == c2); }

inline auto operator+(Color c1, Color c2) -> Color { return c1 += c2; }
inline auto operator-(Color c1, Color c2) -> Color { return c1 -= c2; }
inline auto operator*(Color c, float f) -> Color { return c *= f; }
inline auto operator*(float f, Color c) -> Color { return c *= f; }
inline auto operator*(Color c1, Color c2) -> Color { return c1 *= c2; }

} // namespace raytrace

#endif

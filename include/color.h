#ifndef RAYTRACE_COLOR_H_GUARD
#define RAYTRACE_COLOR_H_GUARD

#include <primitives.h>

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

  friend bool operator==(Color c1, Color c2) {
    return are_about_equal(c1.r, c2.r) && are_about_equal(c1.g, c2.g) &&
           are_about_equal(c1.b, c2.b);
  }

  friend auto operator!=(Color c1, Color c2) -> bool { return !(c1 == c2); }
};

namespace colors {
constexpr auto white = Color{1.0f, 1.0f, 1.0f};
constexpr auto black = Color{0.0f, 0.0f, 0.0f};
} // namespace colors

inline auto operator<<(std::ostream &os, Color const &val) -> std::ostream & {
  os << "Color(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

inline auto operator+(Color c1, Color c2) -> Color { return c1 += c2; }
inline auto operator-(Color c1, Color c2) -> Color { return c1 -= c2; }
inline auto operator*(Color c, float f) -> Color { return c *= f; }
inline auto operator*(float f, Color c) -> Color { return c *= f; }
inline auto operator*(Color c1, Color c2) -> Color { return c1 *= c2; }

} // namespace raytrace

#endif

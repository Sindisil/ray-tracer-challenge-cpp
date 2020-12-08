#ifndef RAYTRACE_COLOR_H
#define RAYTRACE_COLOR_H

#include <ostream>

namespace raytrace {

struct Color {
  float r;
  float g;
  float b;

  Color &operator+=(Color c) {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
  }

  Color &operator-=(Color c) {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
  }

  Color &operator*=(float f) {
    r *= f;
    g *= f;
    b *= f;
    return *this;
  }

  Color &operator*=(Color c) {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
  }
};

std::ostream &operator<<(std::ostream &os, Color const &val);

bool operator==(Color c1, Color c2);
inline bool operator!=(Color c1, Color c2) { return !(c1 == c2); }

inline Color operator+(Color c1, Color c2) { return c1 += c2; }
inline Color operator-(Color c1, Color c2) { return c1 -= c2; }
inline Color operator*(Color c, float f) { return c *= f; }
inline Color operator*(float f, Color c) { return c *= f; }
inline Color operator*(Color c1, Color c2) { return c1 *= c2; }

} // namespace raytrace

#endif

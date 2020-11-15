#ifndef PRIMATIVES_H_GUARD
#define PRIMATIVES_H_GUARD

#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <array>

namespace raytrace {

struct Vec3 {
  float x;
  float y;
  float z;

  explicit Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

  Vec3 &operator+=(Vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vec3 &operator-=(Vec3 v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  Vec3 &operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  Vec3 &operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
  }

  float magnitude();
  Vec3 &normalize();

  float dot(Vec3 v) { return (x * v.x) + (y * v.y) + (z * v.z); }
  Vec3 cross(Vec3 v) {
    return Vec3{(y * v.z) - (z * v.y), (z * v.x) - (x * v.z),
                (x * v.y) - (y * v.x)};
  }
};

struct Point {
  float x;
  float y;
  float z;

  explicit Point(float x, float y, float z) : x(x), y(y), z(z) {}
  explicit Point(float x, float y) : Point(x, y, 0) {}

  Point &operator+=(Vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
};

bool operator==(Point const &lhs, Point const &rhs);
bool operator==(Vec3 const &lhs, Vec3 const &rhs);
inline bool operator!=(Point const &lhs, Point const &rhs) {
  return !(lhs == rhs);
}
inline bool operator!=(Vec3 const &lhs, Vec3 const &rhs) {
  return !(lhs == rhs);
}

inline Point operator+(Point p, Vec3 v) { return p += v; }
inline Point operator+(Vec3 v, Point p) { return p += v; }
inline Vec3 operator+(Vec3 v1, Vec3 v2) { return v1 += v2; }

inline Point operator-(Point p, Vec3 v) {
  p.x -= v.x;
  p.y -= v.y;
  p.z -= v.z;
  return p;
}

inline Vec3 operator-(Point p1, Point p2) {
  return Vec3{p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
}
inline Vec3 operator-(Vec3 v1, Vec3 v2) { return v1 -= v2; }

inline Vec3 operator-(Vec3 v) { return Vec3{-v.x, -v.y, -v.z}; }

inline Vec3 operator*(Vec3 v, float f) { return v *= f; }
inline Vec3 operator*(float f, Vec3 v) { return v *= f; }

inline Vec3 operator/(Vec3 v, float f) { return v /= f; }
inline Vec3 operator/(float f, Vec3 v) { return v /= f; }

inline float dot(Vec3 v1, Vec3 v2) { return v1.dot(v2); }
inline Vec3 cross(Vec3 v1, Vec3 v2) { return v1.cross(v2); }

inline std::ostream &operator<<(std::ostream &os, Vec3 const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, Point const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

struct Color {
  float r;
  float g;
  float b;

  Color(float r, float g, float b) : r(r), g(g), b(b) {}

  Color &operator+=(Color c) {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
  }

  Color &operator-=(Color c) {
    r -= c.r;
    g -= c.r;
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

bool operator==(Color c1, Color c2);
inline bool operator!=(Color c1, Color c2) { return !(c1 == c2); }

inline Color operator+(Color c1, Color c2) { return c1 += c2; }
inline Color operator-(Color c1, Color c2) { return c1 -= c2; }
inline Color operator*(Color c, float f) { return c *= f; }
inline Color operator*(float f, Color c) { return c *= f; }
inline Color operator*(Color c1, Color c2) { return c1 *= c2; }

template <size_t R, size_t C> class Matrix {
public:
  Matrix(std::initializer_list<float> vals) {
    int i = 0;
    for (auto vi = std::begin(vals);
         vi != std::end(vals) && i < m_cells.size(); ++vi) {
      m_cells[i] = *vi;
      ++i;
    }
  }
  float &operator()(int r, int c) {
    if (r < 0 || c < 0 || r >= R || c >= C) {
      throw std::out_of_range("index out of range");
    }
    return m_cells.at(r * C + c);
  }

private:
  std::array<float, R*C> m_cells{0};
};

template <size_t R, size_t C>
bool operator==(Matrix<R, C> lhs, Matrix<R, C> rhs);

template <size_t R, size_t C>
inline bool operator!=(Matrix<R, C> lhs, Matrix<R, C> rhs) {
  return !(lhs == rhs);
};

} // namespace raytrace
#endif

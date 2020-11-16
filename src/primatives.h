#ifndef PRIMATIVES_H_GUARD
#define PRIMATIVES_H_GUARD

#include <algorithm>
#include <array>
#include <initializer_list>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace raytrace {

template <typename T, typename std::enable_if_t<
                          std::is_floating_point<T>::value, T> * = nullptr>
inline bool about_equal(T lhs, T rhs,
                        T max_rel_diff = std::numeric_limits<T>::epsilon()) {
  T diff = std::abs(lhs - rhs);
  lhs = std::abs(lhs);
  rhs = std::abs(rhs);
  return diff <= (std::max(lhs, rhs) * max_rel_diff);
}

template <size_t R, size_t C> class Matrix {
public:
  Matrix(std::initializer_list<float> vals) {
    int i = 0;
    for (auto vi = std::begin(vals); vi != std::end(vals) && i < m_cells.size();
         ++vi) {
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

  Matrix<R, C> transpose();

private:
  std::array<float, R * C> m_cells{0};
};

template <size_t R, size_t C>
bool operator==(Matrix<R, C> lhs, Matrix<R, C> rhs);

template <size_t R, size_t C>
inline bool operator!=(Matrix<R, C> lhs, Matrix<R, C> rhs) {
  return !(lhs == rhs);
};

template <size_t R, size_t C, size_t N>
Matrix<R, C> operator*(Matrix<R, N> lhs, Matrix<N, C> rhs);

template <size_t N> Matrix<N, N> identity_matrix();

struct Vec3 {
  float x;
  float y;
  float z;

  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

  // explicit operator Matrix<4, 1>() { return Matrix<4, 1>{x, y, z, 0}; }

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

  Vec3 &normalize() {
    auto mag = magnitude();
    if (mag == 0) {
      throw std::range_error("Can't normalize Vec3 with zero magnitude");
    }
    x /= mag;
    y /= mag;
    z /= mag;
    return *this;
  }

  float magnitude() { return sqrt(x * x + y * y + z * z); }

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

  Point(float x, float y, float z) : x(x), y(y), z(z) {}
  Point(float x, float y) : Point(x, y, 0) {}

  // explicit operator Matrix<4, 1>() { return Matrix<4, 1>{x, y, z, 1}; }

  Point &operator+=(Vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
};

inline bool operator==(Point const &lhs, Point const &rhs) {
  return about_equal(lhs.x, rhs.x) && about_equal(lhs.y, rhs.y) &&
         about_equal(lhs.z, rhs.z);
}

inline bool operator==(Vec3 const &lhs, Vec3 const &rhs) {
  return about_equal(lhs.x, rhs.x) && about_equal(lhs.y, rhs.y) &&
         about_equal(lhs.z, rhs.z);
}

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

inline Vec3 normalize(Vec3 v) { return v.normalize(); }
inline float dot(Vec3 v1, Vec3 v2) { return v1.dot(v2); }
inline Vec3 cross(Vec3 v1, Vec3 v2) { return v1.cross(v2); }

std::ostream &operator<<(std::ostream &os, Vec3 const &val);
std::ostream &operator<<(std::ostream &os, Point const &val);

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

std::ostream &operator<<(std::ostream &os, Color const &val);

bool operator==(Color c1, Color c2);
inline bool operator!=(Color c1, Color c2) { return !(c1 == c2); }

inline Color operator+(Color c1, Color c2) { return c1 += c2; }
inline Color operator-(Color c1, Color c2) { return c1 -= c2; }
inline Color operator*(Color c, float f) { return c *= f; }
inline Color operator*(float f, Color c) { return c *= f; }
inline Color operator*(Color c1, Color c2) { return c1 *= c2; }

template <size_t R, size_t C> Point operator*(Matrix<R, C> lhs, Point p);
template <size_t R, size_t C> Vec3 operator*(Matrix<R, C> lhs, Vec3 v);

} // namespace raytrace
#endif

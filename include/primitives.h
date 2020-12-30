#ifndef RAYTRACE_PRIMITIVES_H_GUARD
#define RAYTRACE_PRIMITIVES_H_GUARD

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace raytrace {

constexpr float epsilon{0.00001f};
const float pi = 2 * acos(0.f);

inline bool are_about_equal(float lhs, float rhs) {
  return std::abs(lhs - rhs) < epsilon;
}

struct Vector3 {
  float x{0};
  float y{0};
  float z{0};

  auto operator+=(Vector3 v) -> Vector3 & {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  auto operator-=(Vector3 v) -> Vector3 & {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  auto operator*=(float f) -> Vector3 & {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  auto operator/=(float f) -> Vector3 & {
    x /= f;
    y /= f;
    z /= f;
    return *this;
  }

  friend auto operator==(Vector3 const &lhs, Vector3 const &rhs) -> bool {
    return are_about_equal(lhs.x, rhs.x) && are_about_equal(lhs.y, rhs.y) &&
           are_about_equal(lhs.z, rhs.z);
  }

  friend auto operator!=(Vector3 const &lhs, Vector3 const &rhs) -> bool {
    return !(lhs == rhs);
  }

  auto normalize() -> Vector3 & {
    auto mag = magnitude();
    if (mag == 0) {
      throw std::range_error("Can't normalize Vector3 with zero magnitude");
    }
    x /= mag;
    y /= mag;
    z /= mag;
    return *this;
  }

  auto magnitude() const -> float { return sqrt(x * x + y * y + z * z); }

  auto dot(Vector3 v) const -> float {
    return (x * v.x) + (y * v.y) + (z * v.z);
  }

  auto cross(Vector3 v) const -> Vector3 {
    return Vector3{(y * v.z) - (z * v.y), (z * v.x) - (x * v.z),
                   (x * v.y) - (y * v.x)};
  }

  auto reflect(Vector3 normal) const -> Vector3;
};

struct Point {
  float x{0};
  float y{0};
  float z{0};

  auto operator+=(Vector3 v) -> Point & {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  auto operator-=(Vector3 v) -> Point & {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  friend auto operator==(Point const &lhs, Point const &rhs) -> bool {
    return are_about_equal(lhs.x, rhs.x) && are_about_equal(lhs.y, rhs.y) &&
           are_about_equal(lhs.z, rhs.z);
  }

  friend auto operator!=(Point const &lhs, Point const &rhs) -> bool {
    return !(lhs == rhs);
  }
};

inline auto operator+(Point p, Vector3 v) -> Point { return p += v; }
inline auto operator+(Vector3 v, Point p) -> Point { return p += v; }
inline auto operator+(Vector3 v1, Vector3 v2) -> Vector3 { return v1 += v2; }

inline auto operator-(Point p, Vector3 v) -> Point { return p -= v; }

inline auto operator-(Point p1, Point p2) -> Vector3 {
  return Vector3{p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
}
inline auto operator-(Vector3 v1, Vector3 v2) -> Vector3 { return v1 -= v2; }

inline auto operator-(Vector3 v) -> Vector3 {
  return Vector3{-v.x, -v.y, -v.z};
}

inline auto operator*(Vector3 v, float f) -> Vector3 { return v *= f; }
inline auto operator*(float f, Vector3 v) -> Vector3 { return v *= f; }

inline auto operator/(Vector3 v, float f) -> Vector3 { return v /= f; }

auto operator<<(std::ostream &os, Vector3 const &val) -> std::ostream &;

auto operator<<(std::ostream &os, Point const &val) -> std::ostream &;

} // namespace raytrace
#endif

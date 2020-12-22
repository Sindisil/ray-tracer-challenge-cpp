#ifndef RAYTRACE_PRIMITIVES_H_GUARD
#define RAYTRACE_PRIMITIVES_H_GUARD

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <stdexcept>

namespace raytrace {

constexpr double epsilon{0.00001};
const double pi = 2 * acos(0.0);

inline bool are_about_equal(double lhs, double rhs) {
  return std::abs(lhs - rhs) < epsilon;
}

struct Vec3 {
  double x{0};
  double y{0};
  double z{0};

  Vec3() = default;
  Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  auto operator+=(Vec3 v) -> Vec3 & {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  auto operator-=(Vec3 v) -> Vec3 & {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  auto operator*=(double f) -> Vec3 & {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  auto operator/=(double f) -> Vec3 & {
    x /= f;
    y /= f;
    z /= f;
    return *this;
  }

  friend auto operator==(Vec3 const &lhs, Vec3 const &rhs) -> bool {
    return are_about_equal(lhs.x, rhs.x) && are_about_equal(lhs.y, rhs.y) &&
           are_about_equal(lhs.z, rhs.z);
  }

  friend auto operator!=(Vec3 const &lhs, Vec3 const &rhs) -> bool {
    return !(lhs == rhs);
  }

  auto normalize() const -> Vec3 {
    auto mag = magnitude();
    if (mag == 0) {
      throw std::range_error("Can't normalize Vec3 with zero magnitude");
    }
    return Vec3(x / mag, y / mag, z / mag);
  }

  auto magnitude() const -> double { return sqrt(x * x + y * y + z * z); }

  auto dot(Vec3 v) const -> double { return (x * v.x) + (y * v.y) + (z * v.z); }

  auto cross(Vec3 v) const -> Vec3 {
    return Vec3{(y * v.z) - (z * v.y), (z * v.x) - (x * v.z),
                (x * v.y) - (y * v.x)};
  }

  auto reflect(Vec3 normal) const -> Vec3;
};

struct Point {
  double x{0};
  double y{0};
  double z{0};

  Point() = default;
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
  Point(double x, double y) : Point(x, y, 0) {}

  auto operator+=(Vec3 v) -> Point & {
    x += v.x;
    y += v.y;
    z += v.z;
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

inline auto operator+(Point p, Vec3 v) -> Point { return p += v; }
inline auto operator+(Vec3 v, Point p) -> Point { return p += v; }
inline auto operator+(Vec3 v1, Vec3 v2) -> Vec3 { return v1 += v2; }

inline auto operator-(Point p, Vec3 v) -> Point {
  p.x -= v.x;
  p.y -= v.y;
  p.z -= v.z;
  return p;
}

inline auto operator-(Point p1, Point p2) -> Vec3 {
  return Vec3{p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
}
inline auto operator-(Vec3 v1, Vec3 v2) -> Vec3 { return v1 -= v2; }

inline auto operator-(Vec3 v) -> Vec3 { return Vec3{-v.x, -v.y, -v.z}; }

inline auto operator*(Vec3 v, double f) -> Vec3 { return v *= f; }
inline auto operator*(double f, Vec3 v) -> Vec3 { return v *= f; }

inline auto operator/(Vec3 v, double f) -> Vec3 { return v /= f; }
inline auto operator/(double f, Vec3 v) -> Vec3 { return v /= f; }

inline auto normalize(Vec3 v) -> Vec3 { return v.normalize(); }
inline auto dot(Vec3 v1, Vec3 v2) -> double { return v1.dot(v2); }
inline auto cross(Vec3 v1, Vec3 v2) -> Vec3 { return v1.cross(v2); }

auto operator<<(std::ostream &os, Vec3 const &val) -> std::ostream &;

auto operator<<(std::ostream &os, Point const &val) -> std::ostream &;

} // namespace raytrace
#endif

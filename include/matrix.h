#ifndef RAYTRACE_MATRIX_H_GUARD
#define RAYTRACE_MATRIX_H_GUARD

#include "primitives.h"

#include <array>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>

namespace raytrace {

struct Mat4;
inline auto operator*(Mat4 lhs, Mat4 rhs) -> Mat4;

struct Mat4 {
  Mat4(std::array<float, 4> r0, std::array<float, 4> r1,
       std::array<float, 4> r2, std::array<float, 4> r3)
      : m_{r0, r1, r2, r3} {}

  Mat4() = default;

  auto operator()(int r, int c) -> float & {
    if (r >= 0 && c >= 0 && r < 4 && c < 4) {
      return m_[r][c];
    } else {
      throw std::out_of_range("matrix index out of range");
    }
  }

  auto operator()(int r, int c) const -> float const & {
    if (r >= 0 && c >= 0 && r < 4 && c < 4) {
      return m_[r][c];
    } else {
      throw std::out_of_range("matrix index out of range");
    }
  }

  friend auto operator==(Mat4 lhs, Mat4 rhs) -> bool {
    for (int r = 0; r < 4; ++r) {
      for (int c = 0; c < 4; ++c) {
        if (!are_about_equal(lhs(r, c), rhs(r, c))) {
          return false;
        }
      }
    }
    return true;
  }

  friend auto operator!=(Mat4 lhs, Mat4 rhs) -> bool { return !(lhs == rhs); }

  auto transpose() const -> Mat4 {
    Mat4 t{};
    for (int c = 0; c < 4; ++c) {
      for (int r = 0; r < 4; ++r) {
        t(c, r) = m_[r][c];
      }
    }
    return t;
  }

  auto isInvertable() const -> bool { return determinant() != 0; }

  auto invert() const -> Mat4 {
    auto m = *this;
    // 2x2 determinants needed to compute larger determinants
    //    names are dCCRR where CC == cols && RR == rows
    auto d2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
    auto d1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
    auto d1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
    auto d0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
    auto d0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
    auto d0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);

    auto det = m(0, 0) * (m(1, 1) * d2323 - m(1, 2) * d1323 + m(1, 3) * d1223) -
               m(0, 1) * (m(1, 0) * d2323 - m(1, 2) * d0323 + m(1, 3) * d0223) +
               m(0, 2) * (m(1, 0) * d1323 - m(1, 1) * d0323 + m(1, 3) * d0123) -
               m(0, 3) * (m(1, 0) * d1223 - m(1, 1) * d0223 + m(1, 2) * d0123);

    if (det == 0) {
      throw std::domain_error("Matrix not invertable");
    }

    auto i_det = 1 / det;

    // Additional 2x2 determinants needed to invert matrix
    //    names are dCCRR where CC == cols && RR == rows
    auto d2313 = m(1, 2) * m(3, 3) - m(1, 3) * m(3, 2);
    auto d1313 = m(1, 1) * m(3, 3) - m(1, 3) * m(3, 1);
    auto d1213 = m(1, 1) * m(3, 2) - m(1, 2) * m(3, 1);
    auto d2312 = m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2);
    auto d1312 = m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1);
    auto d1212 = m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1);
    auto d0313 = m(1, 0) * m(3, 3) - m(1, 3) * m(3, 0);
    auto d0213 = m(1, 0) * m(3, 2) - m(1, 2) * m(3, 0);
    auto d0312 = m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0);
    auto d0212 = m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0);
    auto d0113 = m(1, 0) * m(3, 1) - m(1, 1) * m(3, 0);
    auto d0112 = m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0);

    Mat4 inv{};
    inv(0, 0) = i_det * (m(1, 1) * d2323 - m(1, 2) * d1323 + m(1, 3) * d1223);
    inv(0, 1) = i_det * -(m(0, 1) * d2323 - m(0, 2) * d1323 + m(0, 3) * d1223);
    inv(0, 2) = i_det * (m(0, 1) * d2313 - m(0, 2) * d1313 + m(0, 3) * d1213);
    inv(0, 3) = i_det * -(m(0, 1) * d2312 - m(0, 2) * d1312 + m(0, 3) * d1212);

    inv(1, 0) = i_det * -(m(1, 0) * d2323 - m(1, 2) * d0323 + m(1, 3) * d0223);
    inv(1, 1) = i_det * (m(0, 0) * d2323 - m(0, 2) * d0323 + m(0, 3) * d0223);
    inv(1, 2) = i_det * -(m(0, 0) * d2313 - m(0, 2) * d0313 + m(0, 3) * d0213);
    inv(1, 3) = i_det * (m(0, 0) * d2312 - m(0, 2) * d0312 + m(0, 3) * d0212);

    inv(2, 0) = i_det * (m(1, 0) * d1323 - m(1, 1) * d0323 + m(1, 3) * d0123);
    inv(2, 1) = i_det * -(m(0, 0) * d1323 - m(0, 1) * d0323 + m(0, 3) * d0123);
    inv(2, 2) = i_det * (m(0, 0) * d1313 - m(0, 1) * d0313 + m(0, 3) * d0113);
    inv(2, 3) = i_det * -(m(0, 0) * d1312 - m(0, 1) * d0312 + m(0, 3) * d0112);

    inv(3, 0) = i_det * -(m(1, 0) * d1223 - m(1, 1) * d0223 + m(1, 2) * d0123);
    inv(3, 1) = i_det * (m(0, 0) * d1223 - m(0, 1) * d0223 + m(0, 2) * d0123);
    inv(3, 2) = i_det * -(m(0, 0) * d1213 - m(0, 1) * d0213 + m(0, 2) * d0113);
    inv(3, 3) = i_det * (m(0, 0) * d1212 - m(0, 1) * d0212 + m(0, 2) * d0112);
    return inv;
  }

  auto determinant() const -> float {
    auto m = (*this);
    // 2x2 determinants needed to compute larger determinants
    //    names are dCCRR where CC == cols && RR == rows
    auto d2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
    auto d1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
    auto d1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
    auto d0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
    auto d0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
    auto d0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);

    return m(0, 0) * (m(1, 1) * d2323 - m(1, 2) * d1323 + m(1, 3) * d1223) -
           m(0, 1) * (m(1, 0) * d2323 - m(1, 2) * d0323 + m(1, 3) * d0223) +
           m(0, 2) * (m(1, 0) * d1323 - m(1, 1) * d0323 + m(1, 3) * d0123) -
           m(0, 3) * (m(1, 0) * d1223 - m(1, 1) * d0223 + m(1, 2) * d0123);
  }

  auto translate(float x, float y, float z) const -> Mat4 {
    return Mat4{{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}} *
           (*this);
  } // namespace raytrace

  auto scale(float x, float y, float z) const -> Mat4 {
    return Mat4{{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}} *
           (*this);
  }

  auto rotate_x(float r) const -> Mat4 {
    return Mat4{{1, 0, 0, 0},
                {0, std::cos(r), -std::sin(r), 0},
                {0, std::sin(r), std::cos(r), 0},
                {0, 0, 0, 1}} *
           (*this);
  }

  auto rotate_y(float r) const -> Mat4 {
    return Mat4{{std::cos(r), 0, std::sin(r), 0},
                {0, 1, 0, 0},
                {-std::sin(r), 0, std::cos(r), 0},
                {0, 0, 0, 1}} *
           (*this);
  }

  auto rotate_z(float r) const -> Mat4 {
    return Mat4{{std::cos(r), -std::sin(r), 0, 0},
                {std::sin(r), std::cos(r), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}} *
           (*this);
  }

  auto shear(float xy, float xz, float yx, float yz, float zx, float zy) const
      -> Mat4 {
    return Mat4{{1, xy, xz, 0}, {yx, 1, yz, 0}, {zx, zy, 1, 0}, {0, 0, 0, 1}} *
           (*this);
  }

private:
  std::array<std::array<float, 4>, 4> m_{};
};

// Free functions for Matrix

inline auto identity_matrix() -> Mat4 {
  return Mat4{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
}

inline auto operator*(Mat4 lhs, Mat4 rhs) -> Mat4 {
  Mat4 res{};

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      for (int n = 0; n < 4; ++n) {
        res(r, c) += lhs(r, n) * rhs(n, c);
      }
    }
  }
  return res;
}

inline auto operator*(Mat4 m, Point p) -> Point {
  return Point{m(0, 0) * p.x + m(0, 1) * p.y + m(0, 2) * p.z + m(0, 3),
               m(1, 0) * p.x + m(1, 1) * p.y + m(1, 2) * p.z + m(1, 3),
               m(2, 0) * p.x + m(2, 1) * p.y + m(2, 2) * p.z + m(2, 3)};
}

inline auto operator*(Point p, Mat4 m) -> Point { return m * p; }

inline auto operator*(Mat4 m, Vector3 v) -> Vector3 {
  return Vector3{m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
                 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
                 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z};
}

inline auto operator*(Vector3 v, Mat4 m) -> Vector3 { return m * v; }

inline auto operator<<(std::ostream &os, Mat4 const &val) -> std::ostream & {
  Mat4 &m = const_cast<Mat4 &>(val);
  for (int r = 0; r < 4; ++r) {
    os << (r == 0 ? "" : ", ") << "[";
    for (int c = 0; c < 4; ++c) {
      os << (c == 0 ? "" : ", ") << m(r, c);
    }
    os << "]";
  }
  return os;
}

} // namespace raytrace
#endif // !MATRIX_H

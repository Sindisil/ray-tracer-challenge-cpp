#ifndef RAYTRACE_MATRIX_H_GUARD
#define RAYTRACE_MATRIX_H_GUARD

#include "primitives.h"

#include <cassert>
#include <cmath>
#include <ostream>

namespace raytrace {

template <size_t N> struct Matrix {
public:
  float m_[N][N]{};

  auto operator()(int r, int c) -> float & {
    assert(r >= 0 && c >= 0 && r < int{N} && c < int{N});
    return m_[r][c];
  }

  auto operator()(int r, int c) const -> float const & {
    assert(r >= 0 && c >= 0 && r < int{N} && c < int{N});
    return m_[r][c];
  }

  friend auto operator==(Matrix<N> lhs, Matrix<N> rhs) -> bool {
    for (int r = 0; r < int{N}; ++r) {
      for (int c = 0; c < int{N}; ++c) {
        if (!are_about_equal(lhs(r, c), rhs(r, c))) {
          return false;
        }
      }
    }
    return true;
  }

  friend auto operator!=(Matrix<N> lhs, Matrix<N> rhs) -> bool {
    return !(lhs == rhs);
  }

  auto transpose() const -> Matrix<N> {
    Matrix<N> t{};
    for (int c = 0; c < int{N}; ++c) {
      for (int r = 0; r < int{N}; ++r) {
        t(c, r) = m_[r][c];
      }
    }
    return t;
  }

  template <size_t NN = N, typename std::enable_if<(NN > 1)>::type * = nullptr>
  auto submatrix(int r_skip, int c_skip) const -> Matrix<N - 1> {
    Matrix<N - 1> s{};
    int sRow = 0;
    int sCol = 0;

    if (r_skip > int{N} || c_skip > int{N}) {
      throw std::out_of_range("row or column to skip was out of range");
    }

    for (int r = 0; r < int{N}; ++r) {
      if (r != r_skip) {
        for (int c = 0; c < int{N}; ++c) {
          if (c != c_skip) {
            s(sRow, sCol) = m_[r][c];
            ++sCol;
          }
        }
        ++sRow;
        sCol = 0;
      }
    }
    return s;
  }

  auto isInvertable() const -> bool { return determinant() != 0; }

  auto invert() const -> Matrix<N> {
    if constexpr (N == 4) {
      auto m = *this;
      // 2x2 determinants needed to compute larger determinants
      //    names are dCCRR where CC == cols && RR == rows
      auto d2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
      auto d1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
      auto d1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
      auto d0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
      auto d0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
      auto d0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);

      auto det =
          m(0, 0) * (m(1, 1) * d2323 - m(1, 2) * d1323 + m(1, 3) * d1223) -
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

      Matrix<4> inv{};
      inv(0, 0) = i_det * (m(1, 1) * d2323 - m(1, 2) * d1323 + m(1, 3) * d1223);
      inv(0, 1) =
          i_det * -(m(0, 1) * d2323 - m(0, 2) * d1323 + m(0, 3) * d1223);
      inv(0, 2) = i_det * (m(0, 1) * d2313 - m(0, 2) * d1313 + m(0, 3) * d1213);
      inv(0, 3) =
          i_det * -(m(0, 1) * d2312 - m(0, 2) * d1312 + m(0, 3) * d1212);

      inv(1, 0) =
          i_det * -(m(1, 0) * d2323 - m(1, 2) * d0323 + m(1, 3) * d0223);
      inv(1, 1) = i_det * (m(0, 0) * d2323 - m(0, 2) * d0323 + m(0, 3) * d0223);
      inv(1, 2) =
          i_det * -(m(0, 0) * d2313 - m(0, 2) * d0313 + m(0, 3) * d0213);
      inv(1, 3) = i_det * (m(0, 0) * d2312 - m(0, 2) * d0312 + m(0, 3) * d0212);

      inv(2, 0) = i_det * (m(1, 0) * d1323 - m(1, 1) * d0323 + m(1, 3) * d0123);
      inv(2, 1) =
          i_det * -(m(0, 0) * d1323 - m(0, 1) * d0323 + m(0, 3) * d0123);
      inv(2, 2) = i_det * (m(0, 0) * d1313 - m(0, 1) * d0313 + m(0, 3) * d0113);
      inv(2, 3) =
          i_det * -(m(0, 0) * d1312 - m(0, 1) * d0312 + m(0, 3) * d0112);

      inv(3, 0) =
          i_det * -(m(1, 0) * d1223 - m(1, 1) * d0223 + m(1, 2) * d0123);
      inv(3, 1) = i_det * (m(0, 0) * d1223 - m(0, 1) * d0223 + m(0, 2) * d0123);
      inv(3, 2) =
          i_det * -(m(0, 0) * d1213 - m(0, 1) * d0213 + m(0, 2) * d0113);
      inv(3, 3) = i_det * (m(0, 0) * d1212 - m(0, 1) * d0212 + m(0, 2) * d0112);
      return inv;

    } else {
      if (!isInvertable()) {
        throw std::domain_error("Matrix not invertable");
      }

      Matrix<N> inv{};
      for (int row = 0; row < int{N}; ++row) {
        for (int col = 0; col < int{N}; ++col) {
          auto c = cofactor(row, col);
          inv(col, row) = c / determinant();
        }
      }
      return inv;
    }
  }

  auto minor(int r, int c) const -> float {
    return submatrix(r, c).determinant();
  }

  auto cofactor(int r, int c) const -> float {
    auto f = minor(r, c);
    return ((r + c) % 2) == 0 ? f : -f;
  }

  auto determinant() const -> float {
    auto m = (*this);
    if constexpr (N == 2) {
      return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
    } else if constexpr (N == 4) {
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
    } else {
      float det = 0;
      for (int c = 0; c < int{N}; ++c) {
        det += m(0, c) * cofactor(0, c);
      }
      return det;
    }
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto translate(float x, float y, float z) const -> Matrix<4> {
    return Matrix<4>{{{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto scale(float x, float y, float z) const -> Matrix<N> {
    return Matrix<4>{{{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto rotate_x(float r) const -> Matrix<N> {
    return Matrix<4>{{{1, 0, 0, 0},
                      {0, std::cos(r), -std::sin(r), 0},
                      {0, std::sin(r), std::cos(r), 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto rotate_y(float r) const -> Matrix<N> {
    return Matrix<4>{{{std::cos(r), 0, std::sin(r), 0},
                      {0, 1, 0, 0},
                      {-std::sin(r), 0, std::cos(r), 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto rotate_z(float r) const -> Matrix<N> {
    return Matrix<4>{{{std::cos(r), -std::sin(r), 0, 0},
                      {std::sin(r), std::cos(r), 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  auto shear(float xy, float xz, float yx, float yz, float zx, float zy) const
      -> Matrix<N> {
    return Matrix<4>{
               {{1, xy, xz, 0}, {yx, 1, yz, 0}, {zx, zy, 1, 0}, {0, 0, 0, 1}}} *
           (*this);
  }
};

// Free functions for Matrix

inline auto identity_matrix() -> Matrix<4> {
  return Matrix<4>{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

template <size_t N> auto operator*(Matrix<N> lhs, Matrix<N> rhs) -> Matrix<N> {
  Matrix<N> res{};

  for (int r = 0; r < int{N}; ++r) {
    for (int c = 0; c < int{N}; ++c) {
      for (int n = 0; n < int{N}; ++n) {
        res(r, c) += lhs(r, n) * rhs(n, c);
      }
    }
  }
  return res;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
auto operator*(Matrix<N> m, Point p) -> Point {
  auto prod = Point{};
  prod.x = m(0, 0) * p.x + m(0, 1) * p.y + m(0, 2) * p.z + m(0, 3);
  prod.y = m(1, 0) * p.x + m(1, 1) * p.y + m(1, 2) * p.z + m(1, 3);
  prod.z = m(2, 0) * p.x + m(2, 1) * p.y + m(2, 2) * p.z + m(2, 3);
  return prod;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
auto operator*(Point p, Matrix<N> m) -> Point {
  return m * p;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
auto operator*(Matrix<N> m, Vec3 v) -> Vec3 {
  auto prod = Vec3{};
  prod.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
  prod.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
  prod.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;
  return prod;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
auto operator*(Vec3 v, Matrix<N> m) -> Vec3 {
  return m * v;
}

template <size_t N>
auto operator<<(std::ostream &os, Matrix<N> const &val) -> std::ostream & {
  Matrix<N> &m = const_cast<Matrix<N> &>(val);
  for (int r = 0; r < int{N}; ++r) {
    os << (r == 0 ? "" : ", ") << "[";
    for (int c = 0; c < int{N}; ++c) {
      os << (c == 0 ? "" : ", ") << m(r, c);
    }
    os << "]";
  }
  return os;
}

} // namespace raytrace
#endif // !MATRIX_H

#ifndef MATRIX_H
#define MATRIX_H

#include "primatives.h"

#include <cassert>

// TODO - consider removing most operations from Matrix &
// specializing Matrix as Mat4 for 4x4 matrices. This should
// simplify later code, since most every Matrix we'll use will by 4x4. Might be
// easier than adding the SFINAE guard all every member function of Matrix that
// is only valid for 4x4 matrices.

namespace raytrace {
template <size_t N> struct Matrix {
public:
  float cells[N][N]{};

  float &operator()(int r, int c) {
    if (r < 0 || c < 0 || r >= int{N} || c >= int{N}) {
      throw std::out_of_range("index out of range");
    }
    return cells[r][c];
  }

  Matrix<N> transpose() {
    Matrix<N> t{};
    for (int c = 0; c < int{N}; ++c) {
      for (int r = 0; r < int{N}; ++r) {
        t(c, r) = cells[r][c];
      }
    }
    return t;
  }

  template <size_t NN = N, typename std::enable_if<(NN > 1)>::type * = nullptr>
  Matrix<N - 1> submatrix(int r_skip, int c_skip) {
    Matrix<N - 1> s{};
    int srow = 0;
    int scol = 0;

    if (r_skip > int{N} || c_skip > int{N}) {
      throw std::out_of_range("row or column to skip was out of range");
    }

    for (int r = 0; r < int{N}; ++r) {
      if (r != r_skip) {
        for (int c = 0; c < int{N}; ++c) {
          if (c != c_skip) {
            s(srow, scol) = cells[r][c];
            ++scol;
          }
        }
        ++srow;
        scol = 0;
      }
    }
    return s;
  }

  bool isInvertable() { return determinant() != 0; }

  Matrix<N> inverse() {
    if (!isInvertable()) {
      throw std::domain_error("Matrix not invertable");
    }

    Matrix<N> m{};
    for (int row = 0; row < int{N}; ++row) {
      for (int col = 0; col < int{N}; ++col) {
        auto c = cofactor(row, col);
        m(col, row) = c / determinant();
      }
    }
    return m;
  }

  float minor(int r, int c) { return submatrix(r, c).determinant(); }

  float cofactor(int r, int c) {
    auto f = minor(r, c);
    return (r + c) % 2 == 0 ? f : -f;
  }

  float determinant() {
    if constexpr (N == 2) {
      return ((*this)(0, 0) * (*this)(1, 1)) - ((*this)(0, 1) * (*this)(1, 0));
    } else {
      float det = 0;
      for (int c = 0; c < int{N}; ++c) {
        det += (*this)(0, c) * cofactor(0, c);
      }
      return det;
    }
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<4> translate(float x, float y, float z) {
    return Matrix<4>{{{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> scale(float x, float y, float z) {
    return Matrix<4>{{{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}}} *
           (*this);
  }
};

// Free functions for Matrix

Matrix<4> identityMatrix() {
  return Matrix<4>{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

template <size_t N> bool operator==(Matrix<N> lhs, Matrix<N> rhs);

template <size_t N> inline bool operator!=(Matrix<N> lhs, Matrix<N> rhs) {
  return !(lhs == rhs);
}

template <size_t N> bool operator==(Matrix<N> lhs, Matrix<N> rhs) {
  for (int r = 0; r < int{N}; ++r) {
    for (int c = 0; c < int{N}; ++c) {
      if (!about_equal(lhs(r, c), rhs(r, c))) {
        return false;
      }
    }
  }
  return true;
}

template <size_t N> Matrix<N> operator*(Matrix<N> lhs, Matrix<N> rhs) {
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
Point operator*(Matrix<N> m, Point p) {
  auto prod = Point{};
  prod.x = m(0, 0) * p.x + m(0, 1) * p.y + m(0, 2) * p.z + m(0, 3);
  prod.y = m(1, 0) * p.x + m(1, 1) * p.y + m(1, 2) * p.z + m(1, 3);
  prod.z = m(2, 0) * p.x + m(2, 1) * p.y + m(2, 2) * p.z + m(2, 3);
  return prod;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
Point operator*(Point p, Matrix<N> m) {
  return m * p;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
Vec3 operator*(Matrix<N> m, Vec3 v) {
  auto prod = Vec3{};
  prod.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
  prod.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
  prod.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;
  return prod;
}

template <size_t N, typename std::enable_if<(N == 4)>::type * = nullptr>
Vec3 operator*(Vec3 v, Matrix<N> m) {
  return m * v;
}

template <size_t N>
std::ostream &operator<<(std::ostream &os, Matrix<N> const &val) {
  Matrix<N> &m = const_cast<Matrix<N> &>(val);
  for (int r = 0; r < int{N}; ++r) {
    os << (r == 0 ? "" : ", ") << "(";
    for (int c = 0; c < int{N}; ++c) {
      os << (c == 0 ? "" : ", ") << m(r, c);
    }
    os << ")";
  }
  return os;
}

} // namespace raytrace
#endif // !MATRIX_H

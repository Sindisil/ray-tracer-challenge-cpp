#ifndef MATRIX_H
#define MATRIX_H

#include "primitives.h"

#include <cmath>
#include <ostream>

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

  Matrix<N> transpose() const {
    Matrix<N> t{};
    for (int c = 0; c < int{N}; ++c) {
      for (int r = 0; r < int{N}; ++r) {
        t(c, r) = cells[r][c];
      }
    }
    return t;
  }

  template <size_t NN = N, typename std::enable_if<(NN > 1)>::type * = nullptr>
  Matrix<N - 1> submatrix(int r_skip, int c_skip) const {
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
            s(sRow, sCol) = cells[r][c];
            ++sCol;
          }
        }
        ++sRow;
        sCol = 0;
      }
    }
    return s;
  }

  bool isInvertable() const { return determinant() != 0; }

  Matrix<N> invert() const {
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

  float minor(int r, int c) const { return submatrix(r, c).determinant(); }

  float cofactor(int r, int c) const {
    auto f = minor(r, c);
    return (r + c) % 2 == 0 ? f : -f;
  }

  float determinant() const {
    if constexpr (N == 2) {
      return (cells[0][0] * cells[1][1]) - (cells[0][1] * cells[1][0]);
    } else {
      float det = 0;
      for (int c = 0; c < int{N}; ++c) {
        det += cells[0][c] * cofactor(0, c);
      }
      return det;
    }
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<4> translate(float x, float y, float z) const {
    return Matrix<4>{{{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> scale(float x, float y, float z) const {
    return Matrix<4>{{{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> rotate_x(float r) const {
    return Matrix<4>{{{1, 0, 0, 0},
                      {0, std::cos(r), -std::sin(r), 0},
                      {0, std::sin(r), std::cos(r), 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> rotate_y(float r) const {
    return Matrix<4>{{{std::cos(r), 0, std::sin(r), 0},
                      {0, 1, 0, 0},
                      {-std::sin(r), 0, std::cos(r), 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> rotate_z(float r) const {
    return Matrix<4>{{{std::cos(r), -std::sin(r), 0, 0},
                      {std::sin(r), std::cos(r), 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1}}} *
           (*this);
  }

  template <size_t NN = N, typename std::enable_if<NN == 4>::type * = nullptr>
  Matrix<N> shear(float xy, float xz, float yx, float yz, float zx,
                  float zy) const {
    return Matrix<4>{
               {{1, xy, xz, 0}, {yx, 1, yz, 0}, {zx, zy, 1, 0}, {0, 0, 0, 1}}} *
           (*this);
  }
};

// Free functions for Matrix

inline Matrix<4> identity_matrix() {
  return Matrix<4>{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

template <size_t N> bool operator==(Matrix<N> lhs, Matrix<N> rhs);

template <size_t N> inline bool operator!=(Matrix<N> lhs, Matrix<N> rhs) {
  return !(lhs == rhs);
}

template <size_t N> bool operator==(Matrix<N> lhs, Matrix<N> rhs) {
  for (int r = 0; r < int{N}; ++r) {
    for (int c = 0; c < int{N}; ++c) {
      if (!are_about_equal(lhs(r, c), rhs(r, c))) {
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
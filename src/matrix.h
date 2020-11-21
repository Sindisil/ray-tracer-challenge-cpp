#ifndef MATRIX_H
#define MATRIX_H

#include "primatives.h"

namespace raytrace {
template <size_t R, size_t C> class Matrix {
public:
  template <size_t RR = R, size_t CC = C,
            typename std::enable_if<(RR == 4 && CC == 1) ||
                                    (RR == CC && RR >= 2 && RR <= R)>::type * =
                nullptr>
  Matrix(){};

  template <size_t RR = R, size_t CC = C,
            typename std::enable_if<(RR == 4 && CC == 1) ||
                                    (RR == CC && RR >= 2 && RR <= R)>::type * =
                nullptr>
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

  Matrix<R, C> transpose() {
    Matrix<R, C> t{};
    for (int c = 0; c < C; ++c) {
      for (int r = 0; r < C; ++r) {
        t(c, r) = m_cells[r * C + c];
      }
    }
    return t;
  }

  Matrix<R - 1, C - 1> submatrix(int r_skip, int c_skip) {
    Matrix<R - 1, C - 1> s{};
    int srow = 0;
    int scol = 0;

    if (r_skip > R || c_skip >> C) {
      throw std::out_of_range("row or column to skip was out of range");
    }

    for (int r = 0; r < R; ++r) {
      if (r != r_skip) {
        for (int c = 0; c < C; ++c) {
          if (c != c_skip) {
            s(srow, scol) = m_cells[r * C + c];
            ++scol;
          }
        }
        ++srow;
        scol = 0;
      }
    }
    return s;
  }

  bool is_invertable() { return R == C && determinant() != 0; }

  Matrix<R, C> inverse() {
    if (!is_invertable()) {
      throw std::domain_error("Matrix not invertable");
    }

    Matrix<R, C> m{};
    for (int row = 0; row < R; ++row) {
      for (int col = 0; col < C; ++col) {
        auto c = cofactor(row, col);
        m(col, row) = c / determinant();
      }
    }
    return m;
  }

  template <size_t RR = R, size_t CC = C,
            typename std::enable_if<RR == CC>::type * = nullptr>
  float minor(int r, int c) {
    return submatrix(r, c).determinant();
  }

  template <size_t RR = R, size_t CC = C,
            typename std::enable_if<RR == CC>::type * = nullptr>
  float cofactor(int r, int c) {
    auto f = minor(r, c);
    return (r + c) % 2 == 0 ? f : -f;
  }

  template <size_t RR = R, size_t CC = C,
            typename std::enable_if<RR == CC>::type * = nullptr>
  float determinant() {
    if constexpr (R == 2) {
      return ((*this)(0, 0) * (*this)(1, 1)) - ((*this)(0, 1) * (*this)(1, 0));
    } else {
      float det = 0;
      for (int c = 0; c < R; ++c) {
        det += (*this)(0, c) * cofactor(0, c);
      }
      return det;
    }
  }

private:
  std::array<float, R * C> m_cells{0};
};

template <size_t N> Matrix<N, N> identity_matrix() {
  Matrix<N, N> m{};
  for (int i = 0; i < static_cast<int>(N); ++i) {
    m(i, i) = 1;
  }
  return m;
}

Matrix<4, 1> from_point(Point p) { return Matrix<4, 1>{p.x, p.y, p.z, 1}; }
Matrix<4, 1> from_vec3(Vec3 p) { return Matrix<4, 1>{p.x, p.y, p.z, 0}; }

template <size_t R, size_t C>
bool operator==(Matrix<R, C> lhs, Matrix<R, C> rhs);

template <size_t R, size_t C>
inline bool operator!=(Matrix<R, C> lhs, Matrix<R, C> rhs) {
  return !(lhs == rhs);
};

template <size_t R, size_t C>
bool operator==(Matrix<R, C> lhs, Matrix<R, C> rhs) {
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (!about_equal(lhs(r, c), rhs(r, c))) {
        return false;
      }
    }
  }
  return true;
}

template <size_t R, size_t C, size_t N>
Matrix<R, C> operator*(Matrix<R, N> lhs, Matrix<N, C> rhs) {
  Matrix<R, C> res{};

  for (int r = 0; r < static_cast<int>(R); ++r) {
    for (int c = 0; c < static_cast<int>(C); ++c) {
      for (int n = 0; n < static_cast<int>(N); ++n) {
        res(r, c) += lhs(r, n) * rhs(n, c);
      }
    }
  }
  return res;
}

template <size_t R, size_t C> Point operator*(Matrix<R, C> lhs, Point p) {
  auto rhs = from_point(p);
  Matrix<4, 1> prod = lhs * rhs;
  return Point{prod(0, 0), prod(1, 0), prod(2, 0)};
}

template <size_t R, size_t C> Vec3 operator*(Matrix<R, C> lhs, Vec3 v) {
  auto rhs = from_vec3(v);
  Matrix<4, 1> prod = lhs * rhs;
  return Vec3{prod(0, 0), prod(1, 0), prod(2, 0)};
}

template <size_t R, size_t C>
std::ostream &operator<<(std::ostream &os, Matrix<R, C> const &val) {
  Matrix<R, C> &m = const_cast<Matrix<R, C> &>(val);
  for (int r = 0; r < static_cast<int>(R); ++r) {
    os << (r == 0 ? "" : ", ") << "(";
    for (int c = 0; c < static_cast<int>(C); ++c) {
      os << (c == 0 ? "" : ", ") << m(r, c);
    }
    os << ")";
  }
  return os;
}

inline Matrix<4, 4> translation(float x, float y, float z) {
  auto m = identity_matrix<4>();
  m(0, 3) = x;
  m(1, 3) = y;
  m(2, 3) = z;
  return m;
}
} // namespace raytrace
#endif // !MATRIX_H
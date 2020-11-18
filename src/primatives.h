#ifndef PRIMATIVES_H_GUARD
#define PRIMATIVES_H_GUARD

#include <algorithm>
#include <array>
#include <initializer_list>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace raytrace {

inline bool about_equal(float lhs, float rhs) {
  return std::abs(lhs - rhs) < .00001f;
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

  float cofactor(int r, int c) {
    if constexpr (R == C) {
      auto f = submatrix(r, c).determinant();
      return (r + c) % 2 == 0 ? f : -f;
    } else {
      throw std::domain_error("Can't find cofactor of non-square matrix");
    }
  }

  float determinant() {
    if constexpr (R != C) {
      throw std::domain_error("Can't find determinant of non-square matrix");
    } else if constexpr (R == 2) {
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

template <size_t R, size_t C>
bool operator==(Matrix<R, C> lhs, Matrix<R, C> rhs);

template <size_t R, size_t C>
inline bool operator!=(Matrix<R, C> lhs, Matrix<R, C> rhs) {
  return !(lhs == rhs);
};

template <size_t R, size_t C, size_t N>
Matrix<R, C> operator*(Matrix<R, N> lhs, Matrix<N, C> rhs);

template <size_t N> Matrix<N, N> identity_matrix();

template <size_t N> inline float minor(Matrix<N, N> m, int r, int c) {
  return m.submatrix(r, c).determinant();
}

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
  Matrix<4, 1> rhs{p.x, p.y, p.z, 0};
  Matrix<4, 1> prod = lhs * rhs;
  return Point{prod(0, 0), prod(1, 0), prod(2, 0)};
}
template <size_t R, size_t C> Vec3 operator*(Matrix<R, C> lhs, Vec3 v) {
  Matrix<4, 1> rhs{v.x, v.y, v.z, 1};
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

template <size_t N> Matrix<N, N> identity_matrix() {
  Matrix<N, N> m{};
  for (int i = 0; i < static_cast<int>(N); ++i) {
    m(i, i) = 1;
  }
  return m;
}

} // namespace raytrace
#endif

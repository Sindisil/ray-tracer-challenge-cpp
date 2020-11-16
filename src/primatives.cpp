#include "primatives.h"

#include "doctest.h"

#include <iostream>

namespace raytrace {

std::ostream &operator<<(std::ostream &os, Vec3 const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Point const &val) {
  os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
  return os;
}

bool operator==(Color c1, Color c2) {
  return about_equal(c1.r, c2.r) && about_equal(c1.r, c2.r) &&
         about_equal(c1.r, c2.r);
}

std::ostream &operator<<(std::ostream &os, Color const &val) {
  os << "(r=" << val.r << ", g=" << val.g << ", b=" << val.b << ")";
  return os;
}

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

template <size_t R, size_t C> Matrix<R, C> Matrix<R, C>::transpose() {
  Matrix<R, C> t{};
  for (int c = 0; c < C; ++c) {
    for (int r = 0; r < C; ++r) {
      t(c, r) = m_cells[r * C + c];
    }
  }
  return t;
}

template <size_t R, size_t C>
Matrix<R - 1, C - 1> Matrix<R, C>::submatrix(int r_skip, int c_skip) {
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

// Point tests

TEST_CASE("Creating a 3d point") {
  SUBCASE("A Point 'a' is constructed with args 4.3, -4.2, 3.1") {
    Point a{4.3f, -4.2f, 3.1f};

    SUBCASE("A valid Point is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

TEST_CASE("Creating a 2d point") {
  SUBCASE("A Point 'a' is constructed with args -2.0, 7.3") {
    Point a{-2.0f, 7.3f};

    SUBCASE("A valid point is constructed") {
      CHECK_EQ(a.x, doctest::Approx(-2.0f));
      CHECK_EQ(a.y, doctest::Approx(7.3f));
      CHECK_EQ(a.z, doctest::Approx(0));
    }
  }
}

TEST_CASE("Points may be compared for equality and non-equality") {
  Point a{5.0f, -3.0f, 17.0f};
  Point b{5.0f, -3.0f, 17.0f};
  Point c{5.0f, 56.0f, -3.0f};

  SUBCASE("a and b should compare equal, but c shouldn't compare as equal "
          "with a or b") {
    CHECK(a == b);
    CHECK_FALSE(a == c);
    CHECK_FALSE(c == b);
  }

  SUBCASE("a and b shouldn't compare non-equal, but c should compare as "
          "non-equal with a or b") {
    CHECK(a == b);
    CHECK_FALSE(a == c);
    CHECK_FALSE(c == b);
  }
}

// Vec3 tests

TEST_CASE("Creating a Vec3") {
  SUBCASE("A Vector 'a' is constructed with args 4.3, -4.2, 3.1") {
    Vec3 a{4.3f, -4.2f, 3.1f};

    SUBCASE("A valid Vector is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

TEST_CASE("Vec3s may be compared for equality and non-equality") {
  Vec3 a{5.0f, -3.0f, 17.0f};
  Vec3 b{5.0f, -3.0f, 17.0f};
  Vec3 c{5.0f, 56.0f, -3.0f};

  SUBCASE("a and b should compare equal, but c shouldn't compare as equal "
          "with a or b") {
    CHECK(a == b);
    CHECK_FALSE(a == c);
    CHECK_FALSE(c == b);
  }

  SUBCASE("a and b shouldn't compare non-equal, but c should compare as "
          "non-equal with a or b") {
    CHECK(a == b);
    CHECK_FALSE(a == c);
    CHECK_FALSE(c == b);
  }
}

TEST_CASE("Subtracting two vectors") {
  Vec3 v1{3.f, 2.f, 1.f};
  Vec3 v2{5.f, 6.f, 7.f};
  Vec3 res_v{-2.f, -4.f, -6.f};

  CHECK(v1 - v2 == res_v);
}

TEST_CASE("Negating a vector") {
  Vec3 v{1, -2, 3};

  CHECK(-v == Vec3{-1, 2, -3});
}

TEST_CASE("Multiplying a vector by a scalar") {
  Vec3 v{1, -2, 3};

  CHECK(v * 3.5f == Vec3{3.5f, -7, 10.5f});
  CHECK(3.5f * v == Vec3{3.5f, -7, 10.5f});
}

TEST_CASE("Multiplying a vector by a fraction") {
  Vec3 v{1, -2, 3};

  CHECK(v * .5f == Vec3{.5f, -1.f, 1.5f});
  CHECK(.5f * v == Vec3{.5f, -1.f, 1.5f});
}

TEST_CASE("Dividing a vector by a scalar") {
  Vec3 v{1, -2, 3};

  CHECK(v / 2 == Vec3{0.5f, -1.f, 1.5f});
}

// Point & Vec3 tests

TEST_CASE("Points and Vec3s may be added together") {
  Point p1{3, -2, 5};
  Vec3 v1{-2, 3, 1};
  Vec3 v2{2, 1.2f, -6.7f};

  Point res_p{1, 1, 6};
  Vec3 res_v{0, 4.2f, -5.7f};

  CHECK(p1 + v1 == res_p);
  CHECK(v1 + p1 == res_p);
  CHECK(v1 + v2 == res_v);
}

TEST_CASE("Subracting two points") {
  Point p1{3, 2, 1};
  Point p2{5, 6, 7};
  Vec3 res_v{-2, -4, -6};

  CHECK(p1 - p2 == res_v);
}

TEST_CASE("Subtracting a vector from a point") {
  Point p{3, 2, 1};
  Vec3 v{5, 6, 7};
  Point res_p{-2, -4, -6};

  CHECK(p - v == res_p);
}

TEST_CASE("Vectors can return their magnitude") {
  CHECK((Vec3{1, 0, 0}).magnitude() == 1);
  CHECK((Vec3{0, 1, 0}).magnitude() == 1);
  CHECK((Vec3{0, 0, 1}).magnitude() == 1);
  CHECK_EQ((Vec3{1, 2, 3}).magnitude(), doctest::Approx(sqrt(14)));
  CHECK_EQ((Vec3{-1, -2, -3}).magnitude(), doctest::Approx(sqrt(14)));
}

TEST_CASE("Non-zero length vectors can be normalized") {
  SUBCASE("Normalizing (0, 0, 0) throws") {
    CHECK_THROWS_AS(normalize(Vec3{0, 0, 0}), std::range_error const &);
  }
  SUBCASE("Normalizing (4, 0, 0) gives (1, 0, 0)") {
    CHECK(normalize(Vec3{4, 0, 0}) == Vec3{1, 0, 0});
  }
  SUBCASE("Normalizing (1, 2, 3) gives (sqrt(14), sqrt(14), sqrt(14))") {
    CHECK(normalize(Vec3{1.f, 2.f, 3.f}) ==
          Vec3{1 / sqrt(14.f), 2 / sqrt(14.f), 3 / sqrt(14.f)});
  }
  SUBCASE("A normalized vector should have a magnitude of 1") {
    CHECK_EQ(normalize(Vec3{1, 2, 3}).magnitude(), doctest::Approx(1.f));
  }
}

TEST_CASE("The dot product of two vectors") {
  CHECK(dot(Vec3{1, 2, 3}, Vec3{2, 3, 4}) == 20);
}

TEST_CASE("The cross product of two vectors") {
  Vec3 a{1, 2, 3};
  Vec3 b{2, 3, 4};

  CHECK(cross(a, b) == Vec3{-1, 2, -1});
  CHECK(cross(b, a) == Vec3{1, -2, 1});
}

// Color tests

TEST_CASE("Color creation") {
  Color c = Color{-.5f, .4f, 1.7f};
  CHECK(c.r == -.5f);
  CHECK(c.g == .4f);
  CHECK(c.b == 1.7f);
}

TEST_CASE("Adding and subtracting colors") {
  Color c1{.9f, .6f, .75};
  Color c2{.7f, .1f, .25f};

  SUBCASE("Adding colors") { CHECK(c1 + c2 == Color{1.6f, .7f, 1.0f}); }
  SUBCASE("Subtracting colors") { CHECK(c1 - c2 == Color{.2f, .5f, .5f}); }
}

TEST_CASE("Multiplying a color by a scalar") {
  Color c{.2f, .3f, .4f};

  CHECK(c * 2 == Color{.4f, .6f, .8f});
  CHECK(2 * c == Color{.4f, .6f, .8f});
}

TEST_CASE("Multiplying colors") {
  Color c1{1.f, .2f, .4f};
  Color c2{.9f, 1.f, .1f};
  CHECK(c1 * c2 == Color{.9f, .2f, .4f});
}

// Matrix tests

TEST_CASE("Constructing and inspecting a 4x4 matrix") {
  Matrix<4, 4> m{1.f, 2.f,  3.f,  4.f,  5.5f,  6.5f,  7.5f,  8.5f,
                 9.f, 10.f, 11.f, 12.f, 13.5f, 14.5f, 15.5f, 16.5f};
  CHECK(m(0, 0) == 1);
  CHECK(m(0, 3) == 4);
  CHECK(m(1, 0) == doctest::Approx(5.5f));
  CHECK(m(1, 2) == doctest::Approx(7.5f));
  CHECK(m(2, 2) == 11);
  CHECK(m(3, 2) == doctest::Approx(15.5f));
};

TEST_CASE("A 2x2 matrix ought to be representable") {
  Matrix<2, 2> m{-3, 5, 1, -2};
  CHECK(m(0, 0) == -3);
  CHECK(m(0, 1) == 5);
  CHECK(m(1, 0) == 1);
  CHECK(m(1, 1) == -2);
};

TEST_CASE("A 3x3 matrix ought to be representable") {
  Matrix<3, 3> m{-3, 5, 0, 1, -2, -7, 0, 1, 1};
  CHECK(m(0, 0) == -3);
  CHECK(m(1, 1) == -2);
  CHECK(m(2, 2) == 1);
};

TEST_CASE("Matrix equality with identical matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  Matrix<4, 4> b{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  CHECK(a == b);
};

TEST_CASE("Matrix equality with different matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  Matrix<4, 4> b{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2.1f};
  CHECK(a != b);
};

TEST_CASE("Multiplying two matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2};
  Matrix<4, 4> b{-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8};
  Matrix<4, 4> c{20, 22, 50,  48,  44, 54, 114, 108,
                 40, 58, 110, 102, 16, 26, 46,  42};

  CHECK(a * b == c);
};

TEST_CASE("Matrix multiplied by a Vector") {
  Matrix<4, 4> a{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
  Vec3 v{1, 2, 3};

  CHECK(a * v == Vec3{18, 24, 33});
};

TEST_CASE("Matrix multiplied by a Point") {
  Matrix<4, 4> a{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
  Point p{1, 2, 3};

  CHECK(a * p == Point{14, 22, 32});
};

TEST_CASE("Multiplying a matrix by the identity matrix") {
  Matrix<4, 4> m{0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32};
  CHECK(m * identity_matrix<4>() == m);
};

TEST_CASE("Multiplying the identity matrix by a Point or Vector") {
  Matrix<4, 4> m = identity_matrix<4>();
  Point p{1, 2, 3};
  Vec3 v{4, 5, 6};

  CHECK(m * p == p);
  CHECK(m * v == v);
};

TEST_CASE("Transposing a matrix") {
  Matrix<4, 4> a{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8};
  Matrix<4, 4> transposed{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8};

  CHECK(a.transpose() == transposed);
};

TEST_CASE("Transposing the identity matrix") {
  Matrix<4, 4> m = identity_matrix<4>();

  CHECK(m.transpose() == identity_matrix<4>());
};

TEST_CASE("Calculating the determinant of a 2x2 matrix") {
  Matrix<2, 2> a{1, 5, -3, 2};

  CHECK(determinant(a) == 17);
};

TEST_CASE("A submatrix of a 3x3 matrix is a 2x3 matrix") {
  Matrix<3, 3> a{1, 5, 0, -3, 2, 7, 0, 6, -3};

  CHECK(a.submatrix(0, 2) == Matrix<2, 2>{-3, 2, 0, 6});
};

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix") {
  Matrix<4, 4> a{-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1};
  CHECK(a.submatrix(2, 1) == Matrix<3, 3>{-6, 1, 6, -8, 8, 6, -7, -1, 1});
};

} // namespace raytrace
#include "doctest.h"

#include "matrix.h"
#include "primatives.h"

using namespace raytrace;

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
}

TEST_CASE("A 2x2 matrix ought to be representable") {
  Matrix<2, 2> m{-3, 5, 1, -2};
  CHECK(m(0, 0) == -3);
  CHECK(m(0, 1) == 5);
  CHECK(m(1, 0) == 1);
  CHECK(m(1, 1) == -2);
}

TEST_CASE("A 3x3 matrix ought to be representable") {
  Matrix<3, 3> m{-3, 5, 0, 1, -2, -7, 0, 1, 1};
  CHECK(m(0, 0) == -3);
  CHECK(m(1, 1) == -2);
  CHECK(m(2, 2) == 1);
}

TEST_CASE("Matrix equality with identical matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  Matrix<4, 4> b{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  CHECK(a == b);
}

TEST_CASE("Matrix equality with different matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2};
  Matrix<4, 4> b{1, 2, 3, 4, 5, 6, 7, 8, 9.1f, 8.1f, 7.1f, 6.1f, 5, 4, 3, 2.1f};
  CHECK(a != b);
}

TEST_CASE("Multiplying two matrices") {
  Matrix<4, 4> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2};
  Matrix<4, 4> b{-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8};
  Matrix<4, 4> c{20, 22, 50,  48,  44, 54, 114, 108,
                 40, 58, 110, 102, 16, 26, 46,  42};

  CHECK(a * b == c);
}

TEST_CASE("Matrix multiplied by a Vector") {
  Matrix<4, 4> a{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
  Vec3 v{1, 2, 3};

  CHECK(a * v == Vec3{14, 22, 32});
}

TEST_CASE("Matrix multiplied by a Point") {
  Matrix<4, 4> a{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
  Point p{1, 2, 3};

  CHECK(a * p == Point{18, 24, 33});
}

TEST_CASE("Multiplying a matrix by the identity matrix") {
  Matrix<4, 4> m{0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32};
  CHECK(m * identityMatrix() == m);
}

TEST_CASE("Multiplying the identity matrix by a Point or Vector") {
  Matrix<4, 4> m = identityMatrix();
  Point p{1, 2, 3};
  Vec3 v{4, 5, 6};

  CHECK(m * p == p);
  CHECK(m * v == v);
}

TEST_CASE("Transposing a matrix") {
  Matrix<4, 4> a{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8};
  Matrix<4, 4> transposed{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8};

  CHECK(a.transpose() == transposed);
}

TEST_CASE("Transposing the identity matrix") {
  Matrix<4, 4> m = identityMatrix();

  CHECK(m.transpose() == identityMatrix());
}

TEST_CASE("Calculating the determinant of a 2x2 matrix") {
  Matrix<2, 2> a{1, 5, -3, 2};

  CHECK(a.determinant() == 17);
}

TEST_CASE("A submatrix of a 3x3 matrix is a 2x3 matrix") {
  Matrix<3, 3> a{1, 5, 0, -3, 2, 7, 0, 6, -3};

  CHECK(a.submatrix(0, 2) == Matrix<2, 2>{-3, 2, 0, 6});
}

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix") {
  Matrix<4, 4> a{-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1};
  CHECK(a.submatrix(2, 1) == Matrix<3, 3>{-6, 1, 6, -8, 8, 6, -7, -1, 1});
}

TEST_CASE("Calculating the minor of a 3x3 matrix") {
  Matrix<3, 3> a{3, 5, 0, 2, -1, -7, 6, -1, 5};

  auto b = a.submatrix(1, 0);
  auto det = b.determinant();
  CHECK_EQ(det, doctest::Approx(25.f));
  CHECK_EQ(a.minor(1, 0), doctest::Approx(25.f));
}

TEST_CASE("Calculating a cofactor of a 3x3 matrix") {
  Matrix<3, 3> a{3, 5, 0, 2, -1, -7, 6, -1, 5};

  CHECK_EQ(a.minor(0, 0), doctest::Approx(-12.f));
  CHECK_EQ(a.cofactor(0, 0), doctest::Approx(-12.f));
  CHECK_EQ(a.minor(1, 0), doctest::Approx(25.f));
  CHECK_EQ(a.cofactor(1, 0), doctest::Approx(-25.f));
}

TEST_CASE("Calculating the determinant of a 3x3 matrix") {
  Matrix<3, 3> a{1, 2, 6, -5, 8, -4, 2, 6, 4};
  CHECK_EQ(a.cofactor(0, 0), doctest::Approx(56.f));
  CHECK_EQ(a.cofactor(0, 1), doctest::Approx(12.f));
  CHECK_EQ(a.cofactor(0, 2), doctest::Approx(-46.f));
  CHECK_EQ(a.determinant(), doctest::Approx(-196.f));
}

TEST_CASE("Calculating the determinant of a 4x4 matrix") {
  Matrix<4, 4> a{-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9};
  CHECK_EQ(a.cofactor(0, 0), doctest::Approx(690.f));
  CHECK_EQ(a.cofactor(0, 1), doctest::Approx(447.f));
  CHECK_EQ(a.cofactor(0, 2), doctest::Approx(210.f));
  CHECK_EQ(a.cofactor(0, 3), doctest::Approx(51.f));
  CHECK_EQ(a.determinant(), doctest::Approx(-4071.f));
}

TEST_CASE("Testing an invertable matrix for invertibility") {
  Matrix<4, 4> a{6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6};
  CHECK_EQ(a.determinant(), doctest::Approx(-2120.f));
  CHECK(a.isInvertable());
}

TEST_CASE("Testing a noninvertable matrix for invertability") {
  Matrix<4, 4> a{-4, 2, -2, 3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0};
  CHECK_EQ(a.determinant(), doctest::Approx(0.f));
  CHECK(!a.isInvertable());
}

TEST_CASE("Calculating the inverse of a matrix") {
  Matrix<4, 4> a{-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4};
  auto b = a.inverse();
  CHECK_EQ(a.determinant(), doctest::Approx(532));
  CHECK_EQ(a.cofactor(2, 3), doctest::Approx(-160));
  CHECK_EQ(b(3, 2), doctest::Approx(-160.f / 532.f));
  CHECK_EQ(a.cofactor(3, 2), doctest::Approx(105.f));
  CHECK_EQ(b(2, 3), doctest::Approx(105.f / 532.f));
  CHECK(b == Matrix<4, 4>{0.21805f, 0.45113f, 0.24060f, -0.04511f, -0.80827f,
                          -1.45677f, -0.44361f, 0.52068f, -0.07895f, -0.22368f,
                          -0.05263f, 0.19737f, -0.52256f, -0.81391f, -0.30075f,
                          0.30639f});
}

TEST_CASE("Calculating the inverse of another matrix") {
  Matrix<4, 4> a{8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4};
  CHECK(a.inverse() == Matrix<4, 4>{-.15385f, -.15385f, -.28205f, -.53846f,
                                    -.07692f, .12308f, .02564f, .03077f,
                                    .35897f, .35897f, .43590f, .92308f,
                                    -.69231f, -.69231f, -.76923f, -1.92308f});
}

TEST_CASE("Multiplying a product by its inverse") {
  Matrix<4, 4> a{
      {{3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 4}, {1, -6, 5, -1}}};
  Matrix<4, 4> b{8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5};
  auto c = a * b;
  CHECK(c * b.inverse() == a);
}

TEST_CASE("Multiplying by a translation matrix") {
  auto transform = identityMatrix().translate(5, -3, 2);
  Point p{-3, 4, 5};

  CHECK(transform * p == Point{2, 1, 7});
}

TEST_CASE("Multiplying by the innverse of a translation matrix") {
  auto inv = identityMatrix().translate(5, -3, 2).inverse();
  auto p = Point{-3, 4, 5};
  CHECK(inv * p == Point{-8, 7, 3});
}

TEST_CASE("Translation does not affect vectors") {
  auto transform = identityMatrix().translate(5, -3, 2);
  auto v = Vec3{-3, 4, 5};
  CHECK(transform * v == v);
}

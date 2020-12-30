#include <cmath>

#include "doctest.h"
#include "matrix.h"
#include "primitives.h"

using namespace raytrace;

// Matrix tests

TEST_CASE("Constructing and inspecting a 4x4 matrix") {
  Matrix4 m{{1.f, 2.f, 3.f, 4.f},
            {5.5f, 6.5f, 7.5f, 8.5f},
            {9.f, 10.f, 11.f, 12.f},
            {13.5f, 14.5f, 15.5f, 16.5f}};

  CHECK(m(0, 0) == 1);
  CHECK(m(0, 3) == 4);
  CHECK(m(1, 0) == doctest::Approx(5.5f));
  CHECK(m(1, 2) == doctest::Approx(7.5f));
  CHECK(m(2, 2) == 11);
  CHECK(m(3, 2) == doctest::Approx(15.5f));
}

TEST_CASE("Matrix access operator subscripts are bounds checked") {
  Matrix4 a{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
  CHECK_THROWS_AS(a(-1, -1), std::out_of_range);
  CHECK_THROWS_AS(a(4, 4), std::out_of_range);
}

TEST_CASE("Matrix equality with identical matrices") {
  auto a = Matrix4{{1.0f, 2.0f, 3.0f, 4.0f},
                   {5.0f, 6.0f, 7.0f, 8.0f},
                   {9.0f, 8.0f, 7.0f, 6.0f},
                   {5.0f, 4.0f, 3.0f, 2.0f}};
  auto b = Matrix4{{1.0f, 2.0f, 3.0f, 4.0f},
                   {5.0f, 6.0f, 7.0f, 8.0f},
                   {9.0f, 8.0f, 7.0f, 6.0f},
                   {5.0f, 4.0f, 3.0f, 2.0f}};
  CHECK(a == b);
}

TEST_CASE("Matrix equality with different matrices") {
  Matrix4 a{{1, 2, 3, 4}, {5, 6, 7, 8}, {9.1f, 8.1f, 7.1f, 6.1f}, {5, 4, 3, 2}};
  Matrix4 b{
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9.1f, 8.1f, 7.1f, 6.1f}, {5, 4, 3, 2.1f}};
  Matrix4 c{
      {1, 2, 3, 4}, {5, 6, 7, 0}, {9.1f, 8.1f, 7.1f, 6.1f}, {5, 4, 3, 2.0f}};
  CHECK(a != b);
  CHECK(a != c);
}

TEST_CASE("Multiplying two matrices") {
  Matrix4 a{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
  Matrix4 b{{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}};
  Matrix4 c{{20, 22, 50, 48},
            {44, 54, 114, 108},
            {40, 58, 110, 102},
            {16, 26, 46, 42}};

  CHECK(a * b == c);
}

TEST_CASE("Matrix multiplied by a Vector") {
  Matrix4 a{{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
  Vector3 v{1, 2, 3};

  CHECK(a * v == Vector3{14, 22, 32});
}

TEST_CASE("Matrix multiplied by a Point") {
  Matrix4 a{{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
  Point p{1, 2, 3};

  CHECK(a * p == Point{18, 24, 33});
}

TEST_CASE("Multiplying a matrix by the identity matrix") {
  Matrix4 m{{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}};
  CHECK(m * identity_matrix() == m);
}

TEST_CASE("Multiplying the identity matrix by a Point or Vector") {
  Matrix4 m = identity_matrix();
  Point p{1, 2, 3};
  Vector3 v{4, 5, 6};

  CHECK(m * p == p);
  CHECK(m * v == v);
}

TEST_CASE("Transposing a matrix") {
  Matrix4 a{{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}};
  Matrix4 transposed{{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}};

  CHECK(a.transpose() == transposed);
}

TEST_CASE("Transposing the identity matrix") {
  Matrix4 m = identity_matrix();

  CHECK(m.transpose() == identity_matrix());
}

TEST_CASE("Calculating the determinant of a 4x4 matrix") {
  Matrix4 a{{-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9}};
  CHECK_EQ(a.determinant(), doctest::Approx(-4071.f));
}

TEST_CASE("Testing an invertable matrix for invertibility") {
  Matrix4 a{{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}};
  CHECK_EQ(a.determinant(), doctest::Approx(-2120.f));
  CHECK(a.isInvertable());
}

TEST_CASE("Testing a noninvertable matrix for invertability") {
  Matrix4 a{{-4, 2, -2, 3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}};
  auto b = identity_matrix();
  CHECK_EQ(a.determinant(), doctest::Approx(0.f));
  CHECK(!a.isInvertable());
}

TEST_CASE("Attempting to invert a noninvertable matrix matrix should throw") {
  Matrix4 a{{-4, 2, -2, 3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}};
  CHECK_THROWS_AS(a.invert(), std::domain_error);
}

TEST_CASE("Calculating the inverse of a matrix") {
  Matrix4 a{{-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4}};
  auto b = a.invert();
  CHECK_EQ(a.determinant(), doctest::Approx(532));
  CHECK_EQ(b(3, 2), doctest::Approx(-160.f / 532.f));
  CHECK_EQ(b(2, 3), doctest::Approx(105.f / 532.f));
  CHECK(b == Matrix4{{0.21805f, 0.45113f, 0.24060f, -0.04511f},
                     {-0.80827f, -1.45677f, -0.44361f, 0.52068f},
                     {-0.07895f, -0.22368f, -0.05263f, 0.19737f},
                     {-0.52256f, -0.81391f, -0.30075f, 0.30639f}});
}

TEST_CASE("Calculating the inverse of another matrix") {
  Matrix4 a{{8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4}};
  CHECK(a.invert() == Matrix4{{-.15385f, -.15385f, -.28205f, -.53846f},
                              {-.07692f, .12308f, .02564f, .03077f},
                              {.35897f, .35897f, .43590f, .92308f},
                              {-.69231f, -.69231f, -.76923f, -1.92308f}});
}

TEST_CASE("Multiplying a product by its inverse") {
  Matrix4 a{{3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 4}, {1, -6, 5, -1}};
  Matrix4 b{{8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5}};
  auto c = a * b;
  CHECK(c * b.invert() == a);
}

TEST_CASE("Multiplying by a translation matrix") {
  auto transform = identity_matrix().translate(5, -3, 2);
  Point p{-3, 4, 5};

  CHECK(transform * p == Point{2, 1, 7});
}

TEST_CASE("Multiplying by the innverse of a translation matrix") {
  auto inv = identity_matrix().translate(5, -3, 2).invert();
  auto p = Point{-3, 4, 5};
  CHECK(inv * p == Point{-8, 7, 3});
}

TEST_CASE("Translation does not affect vectors") {
  auto transform = identity_matrix().translate(5, -3, 2);
  auto v = Vector3{-3, 4, 5};
  CHECK(transform * v == v);
}

TEST_CASE("A scaling matrix applied to a point") {
  Point p{-4, 6, 8};

  CHECK(identity_matrix().scale(2, 3, 4) * p == Point{-8, 18, 32});
}

TEST_CASE("A scaling matrix applied to a vector") {
  Vector3 v{-4, 6, 8};

  CHECK(identity_matrix().scale(2, 3, 4) * v == Vector3{-8, 18, 32});
}

TEST_CASE("Multiplying by the inverse of a scaling matrix") {
  Vector3 v{-4, 6, 8};
  CHECK(identity_matrix().scale(2, 3, 4).invert() * v == Vector3{-2, 2, 2});
}

TEST_CASE("Reflection is scaling by a negative value") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().scale(-1, 1, 1) * p == Point{-2, 3, 4});
}

TEST_CASE("Rotating a point around the x axis") {
  Point p{0, 1, 0};

  CHECK(identity_matrix().rotate_x(pi / 4) * p ==
        Point{0, std::sqrt(2.f) / 2, std::sqrt(2.f) / 2});
  CHECK(identity_matrix().rotate_x(pi / 2) * p == Point{0, 0, 1});
}

TEST_CASE("Inverse of rotate_x rotates the opposite direction") {
  Point p{0, 1, 0};
  CHECK(identity_matrix().rotate_x(pi / 4).invert() * p ==
        Point{0, std::sqrt(2.f) / 2, -std::sqrt(2.f) / 2});
}

TEST_CASE("Rotating a point around the y axis") {
  Point p{0, 0, 1};
  CHECK(identity_matrix().rotate_y(pi / 4) * p ==
        Point{std::sqrt(2.f) / 2, 0, std::sqrt(2.f) / 2});
  CHECK(identity_matrix().rotate_y(pi / 2) * p == Point{1, 0, 0});
}

TEST_CASE("Rotating a point around the z axis") {
  Point p{0, 1, 0};
  CHECK(identity_matrix().rotate_z(pi / 4) * p ==
        Point{-std::sqrt(2.f) / 2, std::sqrt(2.f) / 2, 0});
  CHECK(identity_matrix().rotate_z(pi / 2) * p == Point{-1, 0, 0});
}

TEST_CASE("A shearing transformation moves x in proportion to y") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(1, 0, 0, 0, 0, 0) * p == Point{5, 3, 4});
}

TEST_CASE("A shearing transformation moves x in proportion to z") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(0, 1, 0, 0, 0, 0) * p == Point{6, 3, 4});
}

TEST_CASE("A shearing transformation moves y in proportion to x") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(0, 0, 1, 0, 0, 0) * p == Point{2, 5, 4});
}

TEST_CASE("A shearing transformation moves y in proportion to z") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(0, 0, 0, 1, 0, 0) * p == Point{2, 7, 4});
}

TEST_CASE("A shearing transformation moves z in proportion to x") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(0, 0, 0, 0, 1, 0) * p == Point{2, 3, 6});
}

TEST_CASE("A shearing transformation moves z in proportion to y") {
  Point p{2, 3, 4};
  CHECK(identity_matrix().shear(0, 0, 0, 0, 0, 1) * p == Point{2, 3, 7});
}

TEST_CASE("Transformations are applied in sequence") {
  Point p{1, 0, 1};
  auto a{identity_matrix().rotate_x(pi / 2)};
  auto b{identity_matrix().scale(5, 5, 5)};
  auto c{identity_matrix().translate(10, 5, 7)};

  auto p2{a * p};
  CHECK(p2 == Point{1, -1, 0});
  auto p3{b * p2};
  CHECK(p3 == Point{5, -5, 0});
  auto p4{c * p3};
  CHECK(p4 == Point{15, 0, 7});
}

TEST_CASE("Chained transformations must be appied in the correct order") {
  Point p{1, 0, 1};
  auto a{identity_matrix().rotate_x(pi / 2)};
  auto b{identity_matrix().scale(5, 5, 5)};
  auto c{identity_matrix().translate(10, 5, 7)};

  auto t = c * b * a;
  CHECK(t * p == Point{15, 0, 7});
  CHECK(identity_matrix().rotate_x(pi / 2).scale(5, 5, 5).translate(10, 5, 7) *
            p ==
        Point{15, 0, 7});
}

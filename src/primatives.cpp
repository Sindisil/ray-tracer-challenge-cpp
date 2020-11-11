#include "primatives.h"

#include "doctest.h"

#include <cmath>

constexpr float EPSILON{0.000001f};
bool fequals(float const &lhs, float const &rhs) {
  return fabs(lhs - rhs) < EPSILON;
}

bool operator==(Point const &lhs, Point const &rhs) {
  return fequals(lhs.x, rhs.x) && fequals(lhs.y, rhs.y) &&
         fequals(lhs.z, rhs.z);
}

bool operator==(Vec3 const &lhs, Vec3 const &rhs) {
  return fequals(lhs.x, rhs.x) && fequals(lhs.y, rhs.y) &&
         fequals(lhs.z, rhs.z);
}

float Vec3::magnitude() { return sqrt(x * x + y * y + z * z); }

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

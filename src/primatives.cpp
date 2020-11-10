#include "point.h"
#include "vec3.h"

#include "doctest.h"

#include <cstdlib>

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

#include "doctest.h"

#include "primitives.h"

using namespace raytrace;

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

  SUBCASE("All three components are tested") {
    CHECK(a != Point{5.0f, -3.0f, 0.0f});
    CHECK(a != Point{5.0f, 0.0f, 17.0f});
    CHECK(a != Point{0.0f, -3.0f, 17.0f});
  }
}

// Vec3 tests

TEST_CASE("Creating a Vec3") {
  SUBCASE("A Vector 'a' is constructed with args 4.3, -4.2, 3.1") {
    Vector3 a{4.3f, -4.2f, 3.1f};

    SUBCASE("A valid Vector is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

TEST_CASE("Vec3s may be compared for equality and non-equality") {
  Vector3 a{5.0f, -3.0f, 17.0f};
  Vector3 b{5.0f, -3.0f, 17.0f};
  Vector3 c{5.0f, 56.0f, -3.0f};

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

  SUBCASE("All three components are tested") {
    CHECK(a != Vector3{5.0f, -3.0f, 0.0f});
    CHECK(a != Vector3{5.0f, 0.0f, 17.0f});
    CHECK(a != Vector3{0.0f, -3.0f, 17.0f});
  }
}

TEST_CASE("Subtracting two vectors") {
  Vector3 v1{3.f, 2.f, 1.f};
  Vector3 v2{5.f, 6.f, 7.f};
  Vector3 res_v{-2.f, -4.f, -6.f};

  CHECK(v1 - v2 == res_v);
}

TEST_CASE("Negating a vector") {
  Vector3 v{1, -2, 3};

  CHECK(-v == Vector3{-1, 2, -3});
}

TEST_CASE("Multiplying a vector by a scalar") {
  Vector3 v{1, -2, 3};

  CHECK(v * 3.5f == Vector3{3.5f, -7, 10.5f});
  CHECK(3.5f * v == Vector3{3.5f, -7, 10.5f});
}

TEST_CASE("Multiplying a vector by a fraction") {
  Vector3 v{1, -2, 3};

  CHECK(v * .5f == Vector3{.5f, -1.f, 1.5f});
  CHECK(.5f * v == Vector3{.5f, -1.f, 1.5f});
}

TEST_CASE("Dividing a vector by a scalar") {
  Vector3 v{1, -2, 3};

  CHECK(v / 2 == Vector3{0.5f, -1.f, 1.5f});
}

// Point & Vec3 tests

TEST_CASE("Points and Vec3s may be added together") {
  Point p1{3, -2, 5};
  Vector3 v1{-2, 3, 1};
  Vector3 v2{2, 1.2f, -6.7f};

  Point res_p{1, 1, 6};
  Vector3 res_v{0, 4.2f, -5.7f};

  CHECK(p1 + v1 == res_p);
  CHECK(v1 + p1 == res_p);
  CHECK(v1 + v2 == res_v);
}

TEST_CASE("Subracting two points") {
  Point p1{3, 2, 1};
  Point p2{5, 6, 7};
  Vector3 res_v{-2, -4, -6};

  CHECK(p1 - p2 == res_v);
}

TEST_CASE("Subtracting a vector from a point") {
  Point p{3, 2, 1};
  Vector3 v{5, 6, 7};
  Point res_p{-2, -4, -6};

  CHECK(p - v == res_p);
}

TEST_CASE("Vectors can return their magnitude") {
  CHECK((Vector3{1, 0, 0}).magnitude() == 1);
  CHECK((Vector3{0, 1, 0}).magnitude() == 1);
  CHECK((Vector3{0, 0, 1}).magnitude() == 1);
  CHECK_EQ((Vector3{1, 2, 3}).magnitude(), doctest::Approx(sqrt(14)));
  CHECK_EQ((Vector3{-1, -2, -3}).magnitude(), doctest::Approx(sqrt(14)));
}

TEST_CASE("Non-zero length vectors can be normalized") {
  SUBCASE("Normalizing (0, 0, 0) throws") {
    CHECK_THROWS_AS(Vector3{}.normalize(), std::range_error const &);
  }

  SUBCASE("Normalizing (4, 0, 0) gives (1, 0, 0)") {
    CHECK(Vector3{4, 0, 0}.normalize() == Vector3{1, 0, 0});
  }
  SUBCASE("Normalizing (1, 2, 3) gives (1/sqrt(14), 2/sqrt(14), 3/sqrt(14))") {
    CHECK(Vector3{1.f, 2.f, 3.f}.normalize() ==
          Vector3{1 / sqrt(14.f), 2 / sqrt(14.f), 3 / sqrt(14.f)});
  }
  SUBCASE("A normalized vector should have a magnitude of 1") {
    CHECK_EQ(Vector3{1, 2, 3}.normalize().magnitude(), doctest::Approx(1.f));
  }
}

TEST_CASE("The dot product of two vectors") {
  CHECK(Vector3{1, 2, 3}.dot(Vector3{2, 3, 4}) == 20);
}

TEST_CASE("The cross product of two vectors") {
  Vector3 a{1, 2, 3};
  Vector3 b{2, 3, 4};

  CHECK(a.cross(b) == Vector3{-1, 2, -1});
  CHECK(b.cross(a) == Vector3{1, -2, 1});
}

TEST_CASE("Reflecting a vector approaching at 45°") {
  auto v = Vector3{1, -1, 0};
  auto n = Vector3{0, 1, 0};
  auto r = v.reflect(n);
  CHECK(r == Vector3{1, 1, 0});
}

TEST_CASE("Reflecting a vector off a slanted surface") {
  auto v = Vector3{0, -1, 0};
  auto n = Vector3{std::sqrt(2.0f) / 2, std::sqrt(2.0f) / 2, 0};
  auto r = v.reflect(n);
  CHECK(r == Vector3{1, 0, 0});
}
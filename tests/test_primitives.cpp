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
  Color c1{1.0f, 0.2f, 0.4f};
  Color c2{0.9f, 1.0f, 0.1f};
  CHECK(c1 * c2 == Color{.9f, .2f, .04f});
}

TEST_CASE("Comparing colors") {
  auto c1 = Color{1.0f, 0.2f, 0.4f};
  CHECK(c1 == Color{1.0f, 0.2f, 0.4f});
  CHECK(c1 != Color{1.0f, 0.2f, 0.0f});
  CHECK(c1 != Color{0.0f, 0.2f, 0.4f});
  CHECK(c1 != Color{1.0f, 0.0f, 0.4f});
}

TEST_CASE("Reflecting a vector approaching at 45°") {
  auto v = Vec3{1, -1, 0};
  auto n = Vec3{0, 1, 0};
  auto r = v.reflect(n);
  CHECK(r == Vec3{1, 1, 0});
}

TEST_CASE("Reflecting a vector off a slanted surface") {
  auto v = Vec3{0, -1, 0};
  auto n = Vec3{std::sqrt(2.0f) / 2, std::sqrt(2.0f) / 2, 0};
  auto r = v.reflect(n);
  CHECK(r == Vec3{1, 0, 0});
}
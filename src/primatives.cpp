#include "doctest.h"
#include "vec3.h"
#include "point.h"

SCENARIO("Creating a 3d point") {
  GIVEN("A Point 'a' is constructed with args 4.3, -4.2, 3.1") {
    Point a{4.3f, -4.2f, 3.1f};

    THEN("A valid Point is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

SCENARIO("Creating a 2d point") {
  GIVEN("A Point 'a' is constructed with args -2.0, 7.3") {
    Point a{-2.0f, 7.3f};

    THEN("A valid point is constructed") {
      CHECK_EQ(a.x, doctest::Approx(-2.0f));
      CHECK_EQ(a.y, doctest::Approx(7.3f));
      CHECK_EQ(a.z, doctest::Approx(0));
    }
  }
}


SCENARIO("Creating a Vec3") {
  GIVEN("A Vector 'a' is constructed with args 4.3, -4.2, 3.1") {
    Vec3 a{4.3f, -4.2f, 3.1f};

    THEN("A valid Vector is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

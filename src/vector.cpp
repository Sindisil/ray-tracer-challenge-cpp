#include "vector.h"
#include "doctest.h"

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z){};

SCENARIO("Creating a Vector") {
  GIVEN("A Vector 'a' is constructed with args 4.3, -4.2, 3.1") {
    Vector a{4.3f, -4.2f, 3.1f};

    THEN("A valid Vector is constructed") {
      CHECK_EQ(a.x, doctest::Approx(4.3f));
      CHECK_EQ(a.y, doctest::Approx(-4.2f));
      CHECK_EQ(a.z, doctest::Approx(3.1f));
    }
  }
}

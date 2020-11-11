#include "canvas.h"

#include "doctest.h"

TEST_CASE("Creating a canvas") {
  CHECK_THROWS([]() { Canvas c{0, 1}; }() );

  Canvas c{10, 20};

  bool all_black = true;
  auto black = Color{0.f, 0.f, 0.f};
  for (int x = 0; x < c.width(); ++x) {
    for (int y = 0; y < c.height(); ++y) {
      if (c.pixel_at(x, y) != black) {
        all_black = false;
      }
    }
  }

  CHECK(all_black);
};

TEST_CASE("Writing pixels to a canvas") {
  Canvas c{10, 20};
  Color red{1.f, 0.f, 0.f};

  c.write_pixel(2, 3, red);
  CHECK(c.pixel_at(2, 3) == red);
};
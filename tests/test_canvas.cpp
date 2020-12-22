#include "doctest.h"

#include "canvas.h"

#include <sstream>

using namespace raytrace;

TEST_CASE("Creating a canvas") {
  CHECK_THROWS([]() { Canvas c{0, 1}; }());

  Canvas c{10, 20};

  bool all_black = true;
  auto black = Color{0, 0, 0};
  for (int x = 0; x < c.width(); ++x) {
    for (int y = 0; y < c.height(); ++y) {
      if (c.pixel_at(x, y) != black) {
        all_black = false;
      }
    }
  }

  CHECK(all_black);
}

TEST_CASE("Writing pixels to a canvas") {
  Canvas c{10, 20};
  Color red{1, 0, 0};

  c.write_pixel(2, 3, red);
  CHECK(c.pixel_at(2, 3) == red);
}

TEST_CASE("Constructing the PPM header") {
  Canvas c{5, 3};
  std::istringstream lines{c.to_ppm()};
  std::string line{};

  std::getline(lines, line);
  CHECK(line == "P3");
  std::getline(lines, line);
  CHECK(line == "5 3");
  std::getline(lines, line);
  CHECK(line == "255");
}

TEST_CASE("Constructing the PPM pixel data") {
  Canvas c{5, 3};
  c.write_pixel(0, 0, Color{1.5, 0.0, 0.0});
  c.write_pixel(2, 1, Color{0.0, 0.5, 0.0});
  c.write_pixel(4, 2, Color{-0.5, 0.0, 1.0});

  std::istringstream lines{c.to_ppm()};
  std::string line;

  // skip header
  for (int i = 0; i < 3; ++i) {
    std::getline(lines, line);
  }
  std::getline(lines, line);
  CHECK(line == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
  std::getline(lines, line);
  CHECK(line == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
  std::getline(lines, line);
  CHECK(line == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
}

TEST_CASE("PPM lines > 70 char should be split") {
  Canvas c{10, 2};
  Color fill{1, .8, .6};
  for (int y = 0; y < c.height(); ++y) {
    for (int x = 0; x < c.width(); ++x) {
      c.write_pixel(x, y, fill);
    }
  }
  std::istringstream lines{c.to_ppm()};
  std::string line;

  // skip header
  for (int i = 0; i < 3; ++i) {
    std::getline(lines, line);
  };
  std::getline(lines, line);
  CHECK(line ==
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
  std::getline(lines, line);
  CHECK(line == "153 255 204 153 255 204 153 255 204 153 255 204 153");
  std::getline(lines, line);
  CHECK(line ==
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
  std::getline(lines, line);
  CHECK(line == "153 255 204 153 255 204 153 255 204 153 255 204 153");
}

TEST_CASE("PPM files are terminated by a newline") {
  Canvas c{5, 3};
  CHECK(c.to_ppm().back() == '\n');
}

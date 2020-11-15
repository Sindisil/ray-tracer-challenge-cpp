#include "canvas.h"

#include "doctest.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

namespace raytrace {
std::string Canvas::to_ppm() {
  constexpr auto max_ppm_line_len = 70;
  std::ostringstream ppm;

  // Add PPM header
  ppm << "P3\n"
      << m_width << " " << m_height << "\n"
      << "255\n";

  std::string line;
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      auto c = pixel_at(x, y);
      std::array<int, 3> color_values{static_cast<int>(std::ceil(c.r * 255)),
                                      static_cast<int>(std::ceil(c.g * 255)),
                                      static_cast<int>(std::ceil(c.b * 255))};

      for (int i = 0; i < 3; ++i) {
        auto s = std::to_string(std::clamp(color_values[i], 0, 255));

        // PPM lines should be <= 70 chars
        if (line.size() + s.size() + 1 > max_ppm_line_len) {
          ppm << line << "\n";
          line.clear();
        }
        if (line.size() > 0) {
          line.append(" ");
        }
        line.append(s);
      }
    }
    if (line.size() > 0) {
      ppm << line << "\n";
      line.clear();
    }
  }

  return ppm.str();
}

TEST_CASE("Creating a canvas") {
  CHECK_THROWS([]() { Canvas c{0, 1}; }());

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
};

TEST_CASE("Constructing the PPM pixel data") {
  Canvas c{5, 3};
  c.write_pixel(0, 0, Color(1.5f, 0.f, 0.f));
  c.write_pixel(2, 1, Color(0.f, 0.5f, 0.f));
  c.write_pixel(4, 2, Color(-.5f, 0.f, 1.f));

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
};

TEST_CASE("PPM lines > 70 char should be split") {
  Canvas c{10, 2};
  Color fill{1.f, .8f, .6f};
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
};

TEST_CASE("PPM files are terminated by a newline") {
  Canvas c{5, 3};
  CHECK(c.to_ppm().back() == '\n');
};
} // namespace raytrace
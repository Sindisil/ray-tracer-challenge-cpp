#include "canvas.h"

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

} // namespace raytrace

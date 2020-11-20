#include "canvas.h"
#include "primatives.h"

#include <algorithm>
#include <iostream>

using raytrace::Canvas;
using raytrace::Color;
using raytrace::Point;
using raytrace::Vec3;

struct Projectile {
  Point position;
  Vec3 velocity;

  Projectile(Point position, Vec3 velocity)
      : position(position), velocity(velocity){};
};

struct Environment {
  Vec3 gravity;
  Vec3 wind;

  Environment(Vec3 gravity, Vec3 wind) : gravity(gravity), wind(wind){};
};

Canvas canvas{900, 550};
Color red{255, 0, 0};

Projectile tick(Environment env, Projectile proj) {
  Point pos{proj.position + proj.velocity};
  pos.y = std::max(0.f, pos.y);
  Vec3 vel{proj.velocity + env.gravity + env.wind};
  return Projectile{pos, vel};
}

int main() {
  Environment env{Vec3{0.f, -0.1f, 0.f}, Vec3{-0.01f, 0.f, 0.f}};
  Projectile proj{Point{0, 1, 0}, Vec3{1, 1.8f, 0}.normalize() * 11.25};

  while (proj.position.y > 0) {
    int p_x = static_cast<int>(proj.position.x);
    int p_y = canvas.height() - static_cast<int>(proj.position.y);
    canvas.write_pixel(p_x, p_y, red);
    proj = tick(env, proj);
  }
  std::cout << canvas.to_ppm();
}

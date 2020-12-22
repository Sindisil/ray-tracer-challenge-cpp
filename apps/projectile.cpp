#include "primitives.h"

#include <algorithm>
#include <iostream>

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

Projectile tick(Environment env, Projectile proj) {
  Point pos{proj.position + proj.velocity};
  pos.y = std::max(0.0, pos.y);
  Vec3 vel{proj.velocity + env.gravity + env.wind};
  return Projectile{pos, vel};
}

int main() {
  Environment env{Vec3{0, -0, 0}, Vec3{-0.0, 0, 0}};
  Projectile proj{Point{0, 1, 0}, Vec3{5, 5, 1}.normalize()};

  while (proj.position.y > 0) {
    std::cout << proj.position << "\n";
    proj = tick(env, proj);
  }
  std::cout << proj.position << "\n";
}
#include "primitives.h"

#include <algorithm>
#include <iostream>

using raytrace::Point;
using raytrace::Vector3;

struct Projectile {
  Point position;
  Vector3 velocity;

  Projectile(Point position, Vector3 velocity)
      : position(position), velocity(velocity){};
};

struct Environment {
  Vector3 gravity;
  Vector3 wind;

  Environment(Vector3 gravity, Vector3 wind) : gravity(gravity), wind(wind){};
};

Projectile tick(Environment env, Projectile proj) {
  Point pos{proj.position + proj.velocity};
  pos.y = std::max(0.f, pos.y);
  Vector3 vel{proj.velocity + env.gravity + env.wind};
  return Projectile{pos, vel};
}

int main() {
  Environment env{Vector3{0.f, -0.1f, 0.f}, Vector3{-0.01f, 0.f, 0.f}};
  Projectile proj{Point{0, 1, 0}, Vector3{5, 5, 1}.normalize()};

  while (proj.position.y > 0) {
    std::cout << proj.position << "\n";
    proj = tick(env, proj);
  }
  std::cout << proj.position << "\n";
}
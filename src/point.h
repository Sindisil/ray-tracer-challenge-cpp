#ifndef POINT_C_GUARD
#define POINT_C_GUARD

class Vec3;

class Point {
public:
  float x;
  float y;
  float z;

  Point(float x, float y, float z) : x(x), y(y), z(z){};
  Point(float x, float y) : Point(x, y, 0){};

  Point operator+=(Vec3 v);
};

bool operator==(Point const &lhs, Point const &rhs);
#endif

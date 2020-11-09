#ifndef POINT_C_GUARD
#define POINT_C_GUARD

class Point {
public:
  float x;
  float y;
  float z;

  Point(float x, float y, float z) : x(x), y(y), z(z){};
  Point(float x, float y) : Point(x, y, 0){};
};

#endif

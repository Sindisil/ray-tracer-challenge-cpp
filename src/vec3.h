#ifndef VEC3_C_GUARD
#define VEC3_C_GUARD

class Vec3 {
public:
  float x;
  float y;
  float z;

  Vec3(float x, float y, float z) : x(x), y(y), z(z){};

  Vec3 &operator+=(Vec3 const &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
};

bool operator==(Vec3 const &lhs, Vec3 const &rhs);

#endif

#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include "doctest.h"

#include <atomic>
#include <cstdint>

#include "primitives.h"

namespace raytrace {

static std::atomic<unsigned> next_id(0);

class Sphere {
public:
  Sphere() { m_id = std::atomic_fetch_add(&next_id, 1); }
  unsigned id() const { return m_id; }

private:
  unsigned m_id;
};

inline bool operator==(Sphere lhs, Sphere rhs) { return lhs.id() == rhs.id(); }

std::ostream &operator<<(std::ostream &os, Sphere const &val);

} // namespace raytrace

#endif

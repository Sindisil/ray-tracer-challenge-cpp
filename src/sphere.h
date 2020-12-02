#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include "doctest.h"

#include <atomic>
#include <cstdint>

#include "primatives.h"

namespace raytrace {

static std::atomic<unsigned> next_id(1);

class Sphere {
public:
  Sphere() { m_id = std::atomic_fetch_add(&next_id, 1); }
  unsigned const id() { return m_id; }

private:
  unsigned m_id;
};

} // namespace raytrace

#endif

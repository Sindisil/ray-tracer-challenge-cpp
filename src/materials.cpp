#include "materials.h"

namespace raytrace {

auto lighting(Material material, PointLight light, Point point, Vector3 eye,
              Vector3 normal, bool in_shadow) -> Color {
  auto effective_material_color = material.color() * light.intensity;

  auto color = material.ambient() * effective_material_color;
  if (!in_shadow) {

    auto vec_to_light = (light.position - point).normalize();
    auto light_dot_normal = vec_to_light.dot(normal);
    if (light_dot_normal >= 0) {
      // light on same side of surface as eye, so
      // add diffuse component
      auto diffuse =
          effective_material_color * material.diffuse() * light_dot_normal;
      color += diffuse;

      auto reflect_dot_eye = (-vec_to_light).reflect(normal).dot(eye);
      if (reflect_dot_eye > 0) {
        // reflecting toward eye, so add specular component
        auto factor = std::pow(reflect_dot_eye, material.shininess());
        auto specular = light.intensity * material.specular() * factor;
        color += specular;
      }
    }
  }

  return color;
} // namespace raytrace

} // namespace raytrace

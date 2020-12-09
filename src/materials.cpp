#include "materials.h"

namespace raytrace {

auto lighting(Material material, PointLight light, Point point, Vec3 eye,
              Vec3 normal) -> Color {
  auto effective_material_color = material.color() * light.intensity;

  auto color = material.ambient() * effective_material_color;

  auto vec_to_light = normalize(light.position - point);
  auto light_dot_normal = dot(vec_to_light, normal);
  if (light_dot_normal >= 0) {
    // light on same side of surface as eye, so
    // add diffuse component
    auto diffuse =
        effective_material_color * material.diffuse() * light_dot_normal;
    color += diffuse;

    auto reflect_dot_eye = dot((-vec_to_light).reflect(normal), eye);
    if (reflect_dot_eye > 0) {
      // reflecting toward eye, so add specular component
      auto factor = std::pow(reflect_dot_eye, material.shininess());
      auto specular = light.intensity * material.specular() * factor;
      color += specular;
    }
  }

  return color;
}

} // namespace raytrace

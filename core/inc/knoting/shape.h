#pragma once
#include <PxPhysicsAPI.h>
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>

namespace knot {
namespace components {
class Shape {
   public:
    Shape();
    ~Shape();

    void on_awake();
    void on_destroy();

    std::weak_ptr<PxMaterial_ptr_wrapper> get_material() { return m_material; }
    std::weak_ptr<PxShape_ptr_wrapper> get_shape() { return m_shape; }

    void set_material(std::shared_ptr<PxMaterial_ptr_wrapper> material) { m_material = material; }
    void set_geometry(PxGeometry& geometry);
    void set_local_rotation(quat rotation);

    PxBoxGeometry create_cube_geometry(const vec3& halfsize);
    PxSphereGeometry create_sphere_geometry(const float& radius);
    PxCapsuleGeometry create_capsule_geometry(const float& radius, const float& halfheight);

    std::shared_ptr<PxMaterial_ptr_wrapper> get_PxMaterial_from_pxmaterial();

   protected:
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxShape_ptr_wrapper> m_shape;
    std::shared_ptr<PxMaterial_ptr_wrapper> m_material;
};
}  // namespace components
}  // namespace knot

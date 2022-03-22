#pragma once
#include <PxPhysicsAPI.h>
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/rigidbody.h>

namespace knot {

struct filter_group {
    enum Enum {
        eAll = (1 << 0),
        eParticle = (1 << 1),
    };
};
namespace components {

class Shape {
   public:
    Shape();
    ~Shape();

    void on_awake();
    void on_destroy();
    void on_load();

    std::weak_ptr<PxMaterial_ptr_wrapper> get_material() { return m_material; }
    std::weak_ptr<PxShape_ptr_wrapper> get_shape() { return m_shape; }

    void set_material(std::shared_ptr<PxMaterial_ptr_wrapper> material) { m_material = material; }
    void set_geometry(const PxGeometry& geometry);
    void set_local_rotation(quat rotation);
    void set_filter_data(PxU32 group, PxU32 mask);

    PxBoxGeometry create_cube_geometry(const vec3& halfsize);
    PxSphereGeometry create_sphere_geometry(const float& radius);
    PxCapsuleGeometry create_capsule_geometry(const float& radius, const float& halfheight);

    std::shared_ptr<PxMaterial_ptr_wrapper> get_PxMaterial_from_pxmaterial();

    template <class Archive>
    void save(Archive& archive) const;

    template <class Archive>
    void load(Archive& archive);

   protected:
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxShape_ptr_wrapper> m_shape;
    std::shared_ptr<PxMaterial_ptr_wrapper> m_material;
    vec3 m_shapeSize;
    PxGeometryType::Enum m_shapeType;
};
}  // namespace components
}  // namespace knot

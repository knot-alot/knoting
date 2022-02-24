#pragma once
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>

namespace knot {
namespace components {
class PhysicsMaterial {
   public:
    PhysicsMaterial();
    ~PhysicsMaterial();

    void on_awake();
    void on_destroy();

    std::weak_ptr<PxMaterial_ptr_wrapper> get_px_material() { return m_material; }
    float get_dynamicFriction() { return m_material->get()->getDynamicFriction(); }
    float get_staticFriction() { return m_material->get()->getStaticFriction(); }
    float get_restitution() { return m_material->get()->getRestitution(); }

    void set_dynamicFriction(float dynamicFriction) { m_material->get()->setDynamicFriction(dynamicFriction); }
    void set_staticFriction(float staticFriction) { m_material->get()->setStaticFriction(staticFriction); }
    void set_restitution(float restitution) { m_material->get()->setRestitution(restitution); }
    void set_px_material(float staticFriction, float dynamicFriction, float restitution);

   protected:
    std::shared_ptr<PxMaterial_ptr_wrapper> m_material;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
};
}  // namespace components
}  // namespace knot

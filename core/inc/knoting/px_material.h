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
    float get_dynamic_friction() { return m_material->get()->getDynamicFriction(); }
    float get_static_friction() { return m_material->get()->getStaticFriction(); }
    float get_restitution() { return m_material->get()->getRestitution(); }

    void set_dynamic_friction(float dynamic_friction) { m_material->get()->setDynamicFriction(dynamic_friction); }
    void set_static_friction(float static_friction) { m_material->get()->setStaticFriction(static_friction); }
    void set_restitution(float restitution) { m_material->get()->setRestitution(restitution); }
    void set_px_material(float staticFriction, float dynamicFriction, float restitution);

    template <class Archive>
    void save(Archive& archive) const {
        float dynamicFriction = m_material->get()->getDynamicFriction();
        float staticFriction = m_material->get()->getStaticFriction();
        float restitution = m_material->get()->getRestitution();
        archive(CEREAL_NVP(dynamicFriction), CEREAL_NVP(staticFriction), CEREAL_NVP(restitution));
    }

    template <class Archive>
    void load(Archive& archive) {
        float dynamicFriction, staticFriction, restitution;
        archive(CEREAL_NVP(dynamicFriction), CEREAL_NVP(staticFriction), CEREAL_NVP(restitution));
        on_awake();
        set_px_material(staticFriction, dynamicFriction, restitution);
    }

   protected:
    std::shared_ptr<PxMaterial_ptr_wrapper> m_material;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
};
}  // namespace components
}  // namespace knot

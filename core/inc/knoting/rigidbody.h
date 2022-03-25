#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/component.h>
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/types.h>
#include <iostream>

using namespace physx;

namespace knot {
namespace components {

class RigidBody : public Component<RigidBody> {
    // fllow this way
    // set physics and scene
    // set material(already defalut)
    // set shape
    // set aggregate
    // creat rigidbody

   public:
    RigidBody();
    ~RigidBody();

    void on_awake() override;
    void on_destroy() override;

    void on_load();

    vec3 get_position();
    quat get_rotation();
    bool get_is_dynamic(){return m_isDynamic;}
    std::weak_ptr<PxDynamic_ptr_wrapper> get_dynamic();
    std::weak_ptr<PxStatic_ptr_wrapper> get_static();
    bool get_is_dynamic() { return m_isDynamic; }

    // could use after creat rigid
    void set_transform(const vec3& position, const quat& rotation = quat());
    void set_position(const vec3& position);
    void set_rotation(const quat& rotation);
    void set_name(const std::string& name);
    void set_flag(PxActorFlag::Enum flag);
    void remove_flag(PxActorFlag::Enum flag);
    //

    void create_actor(bool isDynamic, const float& mass = 0);
    void set_shape(std::shared_ptr<PxShape_ptr_wrapper> shape);
    void set_aggregate(std::shared_ptr<PxAggregate_ptr_wrapper> aggragate);

    std::string get_name() { return m_name; }
    PxActorFlags get_flags();
    PxVec3 get_position_from_transform();
    PxQuat get_rotation_from_transform();

    static vec3 PxVec3_to_vec3(PxVec3 v);
    static PxVec3 vec3_to_PxVec3(vec3 v);
    static quat PxQuat_to_quat(PxQuat q);
    static PxQuat quat_to_PxQuat(quat q);

    template <class Archive>
    void save(Archive& archive) const {
        bool isDynamic = false;
        float mass = 0.0f;
        if (m_dynamic) {
            isDynamic = true;
            mass = m_dynamic->get()->getMass();
        }
        archive(cereal::make_nvp("isDynamic", isDynamic), cereal::make_nvp("mass", mass));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(cereal::make_nvp("isDynamic", m_isDynamic), cereal::make_nvp("mass", m_mass));
    }

   private:
    std::shared_ptr<PxShape_ptr_wrapper> get_shape_from_shape();
    std::shared_ptr<PxAggregate_ptr_wrapper> get_aggregate_from_aggregate();

   protected:
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<PxDynamic_ptr_wrapper> m_dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> m_static;
    std::shared_ptr<PxShape_ptr_wrapper> m_shape;
    std::shared_ptr<PxAggregate_ptr_wrapper> m_aggregate;

    float m_mass = 0.0f;
    bool m_isDynamic = false;
    std::string m_name;
};

}  // namespace components
}  // namespace knot

#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/types.h>

using namespace physx;

namespace knot {
namespace components {

class RigidBody {
    // fllow this way
    // set physics and scene
    // set material(already defalut)
    // creat rigidbody

   public:
    RigidBody();
    ~RigidBody();

    void on_awake();
    void on_load();
    void on_destroy();

    vec3 get_position();
    quat get_rotation();
    std::weak_ptr<PxDynamic_ptr_wrapper> get_dynamic();
    std::weak_ptr<PxStatic_ptr_wrapper> get_static();

    // could use after creat rigid
    void set_transform(const vec3& position, const quat& rotation = quat());
    void set_position(const vec3& position);
    void set_rotation(const quat& rotation);
    //

    void create_actor(bool isDynamic, const float& mass = 0);
    void set_shape(std::shared_ptr<PxShape_ptr_wrapper> shape) { m_shape = shape; }

    PxVec3 get_position_from_transform();
    PxQuat get_rotation_from_transform();
    std::shared_ptr<PxShape_ptr_wrapper> get_shape_from_shape();
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

   protected:
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<PxDynamic_ptr_wrapper> m_dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> m_static;
    std::shared_ptr<PxShape_ptr_wrapper> m_shape;

    float m_mass = 0.0f;
    bool m_isDynamic = false;
};

}  // namespace components
}  // namespace knot

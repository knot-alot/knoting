#pragma once

#include <vector>

#include <knoting/event_callback.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/rigidbody.h>
#include <knoting/subsystem.h>
#include <iostream>

namespace knot {

class Engine;

}

namespace knot {

class PhysicsAbstract {
   public:
    PhysicsAbstract() : dynamic(nullptr), statik(nullptr) {}

    std::shared_ptr<PxDynamic_ptr_wrapper> dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> statik;
};

class Physics : public Subsystem {
   public:
    Physics(Engine& engine);
    ~Physics();

    void on_awake() override;
    void on_update(double m_deltatime) override;
    void on_fixed_update() override;
    void on_late_update() override;
    void on_destroy() override;

    void update_info_to_transform();

    std::weak_ptr<PxScene_ptr_wrapper> get_active_Scene() { return m_scene; }
    std::weak_ptr<PxPhysics_ptr_wrapper> get_physics() { return m_physics; }
    PxVec3 get_gravity() { return m_gravity; }
    std::weak_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> get_aggregates() { return m_aggregates; }
    Event_Callback* get_event_callback() { return m_event_callback; }

    void set_gravity(PxVec3 gravity);

    inline static std::unordered_map<uuid, PhysicsAbstract> uuidToAbstract;

   private:
    Engine& m_engine;
    Event_Callback* m_event_callback;
    std::shared_ptr<PxFoundation_ptr_wrapper> m_foundation;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxDispatcher_ptr_wrapper> m_dispatcher;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> m_aggregates;

    PxVec3 m_gravity;
};

}  // namespace knot

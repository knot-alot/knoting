#pragma once

#include <vector>

#include <knoting/px_variables_wrapper.h>
#include <knoting/rigidbody.h>
#include <knoting/subsystem.h>
#include <iostream>

namespace knot {

class Engine;

}

namespace knot {

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

    void set_gravity(PxVec3 gravity);

   private:
    Engine& m_engine;
    std::shared_ptr<PxFoundation_ptr_wrapper> m_foundation;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxDispatcher_ptr_wrapper> m_dispatcher;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> m_aggregates;

    PxVec3 m_gravity;
};

}  // namespace knot

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

    std::weak_ptr<PxScene_ptr_wrapper> get_active_Scene() { return m_Scene; }
    std::weak_ptr<PxPhysics_ptr_wrapper> get_physics() { return m_Physics; }
    PxVec3 get_gravity() { return m_gravity; }
    std::weak_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> get_Aggregates() { return m_Aggregates; }

    void set_gravity(PxVec3 gravity);

   private:
    Engine& m_engine;
    std::shared_ptr<PxFoundation_ptr_wrapper> m_Foundation;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_Physics;
    std::shared_ptr<PxDispatcher_ptr_wrapper> m_Dispatcher;
    std::shared_ptr<PxScene_ptr_wrapper> m_Scene;
    std::shared_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> m_Aggregates;
    PxVec3 m_gravity;
};

}  // namespace knot

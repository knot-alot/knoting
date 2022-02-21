#pragma once

#include <knoting/rigidbody.h>
#include <knoting/subsystem.h>

#define PX_RELEASE(x) \
    if (x) {          \
        x->release(); \
        x = NULL;     \
    }

namespace knot {

class Engine;

}

namespace knot {

class Physics : public Subsystem {
   public:
    Physics(Engine& engine);
    ~Physics();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void update_to_transform();

    PxScene* get_active_Scene() { return mScene; }
    PxPhysics* get_physics() { return mPhysics; }

   private:
    Engine& m_engine;
    PxFoundation* mFoundation;
    PxPhysics* mPhysics;
    PxDefaultCpuDispatcher* mDispatcher;
    PxScene* mScene;
};
}  // namespace knot

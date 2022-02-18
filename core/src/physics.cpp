#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/physics.h>
#include <knoting/scene.h>

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

namespace knot {
Physics::Physics(Engine& engine) : m_engine(engine) {
    mPhysics = NULL;
    mScene = NULL;
    mFoundation = NULL;
    mDispatcher = NULL;
}

Physics::~Physics() {
    PX_RELEASE(mScene);
    PX_RELEASE(mDispatcher);
    PX_RELEASE(mPhysics);
    PX_RELEASE(mFoundation);
}

void Physics::on_awake() {
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), false, NULL);

    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    mDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);
}

void Physics::on_update(double m_delta_time) {
    mScene->simulate(m_delta_time);
    mScene->fetchResults(true);
}

void Physics::on_late_update() {}

void Physics::on_destroy() {}

void Physics::update_to_transform() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto entities = registry.view<components::Transform, components::RigidBody>();

    for (auto& e : entities) {
        auto p0pt = scene.get_game_object_from_handle(e);

        if (!p0pt) {
            continue;
        }

        GameObject p = p0pt.value();
        components::Transform& transform = registry.get<components::Transform>(p.get_handle());
        components::RigidBody& rigidbody = registry.get<components::RigidBody>(p.get_handle());

        transform.set_position(rigidbody.get_position());

        transform.set_rotation(rigidbody.get_rotation());
    }
}

/*
void Physics::on_init() {


}
*/

}  // namespace knot

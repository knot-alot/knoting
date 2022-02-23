#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/physics.h>
#include <knoting/scene.h>

PxDefaultAllocator g_Allocator;
PxDefaultErrorCallback g_ErrorCallback;

namespace knot {
Physics::Physics(Engine& engine)
    : m_engine(engine), m_Physics(nullptr), m_Scene(nullptr), m_Foundation(nullptr), m_Dispatcher(nullptr) {}

Physics::~Physics() {}

void Physics::on_awake() {
    m_Foundation = std::make_shared<PxFoundation_ptr_wrapper>(
        PxCreateFoundation(PX_PHYSICS_VERSION, g_Allocator, g_ErrorCallback));

    m_Physics = std::make_shared<PxPhysics_ptr_wrapper>(
        PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation->get(), PxTolerancesScale(), false, NULL));
    constexpr float defalut_gravity = -9.81f;
    PxSceneDesc sceneDesc(m_Physics->get()->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0, defalut_gravity, 0);
    m_Dispatcher = std::make_shared<PxDispatcher_ptr_wrapper>(PxDefaultCpuDispatcherCreate(2));
    sceneDesc.cpuDispatcher = m_Dispatcher->get();
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_Scene = std::make_shared<PxScene_ptr_wrapper>(m_Physics->get()->createScene(sceneDesc));
}

void Physics::on_update(double m_deltatime) {
    m_Scene->get()->simulate(m_deltatime);
    m_Scene->get()->fetchResults(true);
}

void Physics::on_late_update() {}

void Physics::on_destroy() {}

void Physics::update_info_to_transform() {
    // update the global pos to transform component

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto entities = registry.view<components::Transform, components::RigidBody>();

    for (auto& e : entities) {
        auto goOpt = scene.get_game_object_from_handle(e);

        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        components::Transform& transform = registry.get<components::Transform>(go.get_handle());
        components::RigidBody& rigidbody = registry.get<components::RigidBody>(go.get_handle());
        vec3 pos = rigidbody.get_position();
        quat r = rigidbody.get_rotation();

        transform.set_position(rigidbody.get_position());

        transform.set_rotation(rigidbody.get_rotation());
    }
}

void Physics::set_gravity(PxVec3 gravity) {
    gravity = gravity;
    m_Scene->get()->setGravity(gravity);
}

}  // namespace knot

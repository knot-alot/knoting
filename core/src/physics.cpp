#include <knoting/engine.h>
#include <knoting/physics.h>
#include <knoting/scene.h>

PxDefaultAllocator g_Allocator;
PxDefaultErrorCallback g_ErrorCallback;

namespace knot {
Physics::Physics(Engine& engine)
    : m_engine(engine), m_physics(nullptr), m_scene(nullptr), m_foundation(nullptr), m_dispatcher(nullptr) {}

Physics::~Physics() {}

void Physics::on_awake() {
    m_foundation = std::make_shared<PxFoundation_ptr_wrapper>(
        PxCreateFoundation(PX_PHYSICS_VERSION, g_Allocator, g_ErrorCallback));

    m_physics = std::make_shared<PxPhysics_ptr_wrapper>(
        PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation->get(), PxTolerancesScale(), false, NULL));
    constexpr float defalut_gravity = -9.81f;
    PxSceneDesc sceneDesc(m_physics->get()->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0, defalut_gravity, 0);
    m_dispatcher = std::make_shared<PxDispatcher_ptr_wrapper>(PxDefaultCpuDispatcherCreate(2));
    sceneDesc.cpuDispatcher = m_dispatcher->get();
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_scene = std::make_shared<PxScene_ptr_wrapper>(m_physics->get()->createScene(sceneDesc));

    PxAggregate* aggregate = m_physics->get()->createAggregate(128, false);
    std::shared_ptr<PxAggregate_ptr_wrapper> ag = std::make_shared<PxAggregate_ptr_wrapper>(aggregate, "default");
    std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>> ags;
    ags.push_back(ag);
    m_aggregates = std::make_shared<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>>(ags);
    m_scene->get()->addAggregate(*ag->get_aggregate());
}

void Physics::on_update(double m_deltatime) {}

void Physics::on_fixed_update() {
    constexpr float timestep = 1.0 / 120.0f;
    m_scene->get()->simulate(timestep);
    m_scene->get()->fetchResults(true);
    update_info_to_transform();
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
    m_scene->get()->setGravity(gravity);
}
}  // namespace knot

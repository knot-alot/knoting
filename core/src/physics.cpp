#include <knoting/engine.h>
#include <knoting/physics.h>
#include <knoting/scene.h>
PxDefaultAllocator g_Allocator;
PxDefaultErrorCallback g_ErrorCallback;

PxFilterFlags SampleFilterShader(PxFilterObjectAttributes attributes0,
                                 PxFilterData filterData0,
                                 PxFilterObjectAttributes attributes1,
                                 PxFilterData filterData1,
                                 PxPairFlags& pairFlags,
                                 const void* constantBlock,
                                 PxU32 constantBlockSize) {
    // let triggers through
    if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)) {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }
    // generate contacts for all that were not filtered above
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    // trigger the contact callback for pairs (A,B) where
    // the filtermask of A contains the ID of B and vice versa.
    if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST |
                     PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_CONTACT_POINTS;
        return PxFilterFlag::eDEFAULT;
    }

    return PxFilterFlag::eKILL;
}

namespace knot {
Physics::Physics(Engine& engine)
    : m_engine(engine),
      m_physics(nullptr),
      m_scene(nullptr),
      m_foundation(nullptr),
      m_dispatcher(nullptr),
      m_event_callback(nullptr) {}

Physics::~Physics() {}

void Physics::on_awake() {
    // m_event_callback = std::make_shared<event_callback_ptr_wrapper>();
    m_event_callback = new Event_Callback();

    m_foundation = std::make_shared<PxFoundation_ptr_wrapper>(
        PxCreateFoundation(PX_PHYSICS_VERSION, g_Allocator, g_ErrorCallback));

    m_physics = std::make_shared<PxPhysics_ptr_wrapper>(
        PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation->get(), PxTolerancesScale(), false, NULL));
    constexpr float defalut_gravity = -9.81f * 3.0f;
    PxSceneDesc sceneDesc(m_physics->get()->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0, defalut_gravity, 0);
    m_dispatcher = std::make_shared<PxDispatcher_ptr_wrapper>(PxDefaultCpuDispatcherCreate(2));
    sceneDesc.cpuDispatcher = m_dispatcher->get();
    sceneDesc.filterShader = SampleFilterShader;
    sceneDesc.simulationEventCallback = m_event_callback;
    m_scene = std::make_shared<PxScene_ptr_wrapper>(m_physics->get()->createScene(sceneDesc));

    PxAggregate* aggregate = m_physics->get()->createAggregate(128, false);
    std::shared_ptr<PxAggregate_ptr_wrapper> ag = std::make_shared<PxAggregate_ptr_wrapper>(aggregate, "default");
    std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>> ags;
    ags.push_back(ag);
    m_aggregates = std::make_shared<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>>(ags);
    m_scene->get()->addAggregate(*ag->get_aggregate());

    // m_scene->get()->setSimulationEventCallback(m_event_callback->get());
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

    auto raycast = registry.view<components::Raycast>();
    for(auto& ray:raycast){
        auto goOpt = scene.get_game_object_from_handle(ray);

        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        components::Raycast& r = registry.get<components::Raycast>(go.get_handle());
        r.raycast();
    }
}

void Physics::set_gravity(PxVec3 gravity) {
    gravity = gravity;
    m_scene->get()->setGravity(gravity);
}

}  // namespace knot

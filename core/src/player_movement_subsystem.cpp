#include "knoting/player_movement_subsystem.h"
#include <knoting/engine.h>
#include <knoting/input_manager.h>
namespace knot {
PlayerMovement::~PlayerMovement() {}
PlayerMovement::PlayerMovement(Engine& engine) : m_engine(engine), m_inManager(nullptr) {}

void PlayerMovement::on_awake() {
    m_inManager = m_engine.get_window_module().lock()->get_input_manager();
}
void PlayerMovement::on_update(double m_delta_time) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto players = registry.view<components::Transform, components::Tag, components::RigidController>();

    for (auto player : players) {
        auto goOpt = scene.get_game_object_from_handle(player);

        auto go = goOpt.value();

        auto tag = go.get_component<components::Tag>();
        if (tag.get_id() != tag.get_id_from_tag("PLAYER")) {
            continue;
        }

        auto transform = go.get_component<components::Transform>();
        vec3 playerInputs = transform.get_rotation() * player_inputs();

        auto controller = go.get_component<components::RigidController>();
        if (m_inManager->key_on_release(KeyCode::Space)) {
            controller.add_force(vec3(0, 1, 0) * jumpForce);
            log::debug("jumpi");
        }

        if (glm::abs(playerInputs.x + playerInputs.y + playerInputs.z) <= 0.1f) {
            continue;
        }
        vec3 normInp = glm::normalize(playerInputs);
        const vec3 directionForce =
            ((float)m_delta_time) * m_baseSpeedMulti * m_speedMulti * vec3(normInp.x, 0, normInp.z);

        controller.add_force(directionForce);
    }
}

vec3 PlayerMovement::player_inputs() {
    vec3 m_playerInputs = vec3();
    if (m_inManager->key_pressed(KeyCode::A)) {
        m_playerInputs.x -= 1;
    }
    if (m_inManager->key_pressed(KeyCode::D)) {
        m_playerInputs.x += 1;
    }
    if (m_inManager->key_pressed(KeyCode::W)) {
        m_playerInputs.z += 1;
    }
    if (m_inManager->key_pressed(KeyCode::S)) {
        m_playerInputs.z -= 1;
    }
    return m_playerInputs;
}

void PlayerMovement::on_late_update() {}
void PlayerMovement::on_destroy() {}
bool PlayerMovement::can_jump() {
    // TODO:: check with raycast if you are touching ground
    return true;
}

}  // namespace knot

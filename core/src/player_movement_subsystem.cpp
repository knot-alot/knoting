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
    player_movement(m_delta_time);
    camera_movement(m_delta_time);
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

    bool padPresent = m_inManager->pad_present(PadCode::Pad0);
    if (!padPresent) {
        return m_playerInputs;
    }
    vec2 left = vec2(m_inManager->get_pad_axis(PadCode::Pad0, JoyStickCode::LeftX),
                     m_inManager->get_pad_axis(PadCode::Pad0, JoyStickCode::LeftY));
    m_playerInputs.x = left.x;
    m_playerInputs.y = 0;
    m_playerInputs.z = left.y;

    return m_playerInputs;
}

void PlayerMovement::on_late_update() {}
void PlayerMovement::on_destroy() {}
bool PlayerMovement::can_jump() {
    // TODO:: check with raycast if you are touching ground
    return true;
}
void PlayerMovement::player_movement(double m_delta_time) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    // PLAYER MOVEMENT CODE

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
void PlayerMovement::camera_movement(double m_delta_time) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto cams = registry.view<components::Transform, components::EditorCamera>();

    for (auto& cam : cams) {
        //        vec2 mousePos = m_inManager->get_relative_position();
        //        auto goOpt = scene.get_game_object_from_handle(cam);
        //
        //        auto go = goOpt.value();
        //
        //        auto& transform = go.get_component<components::Transform>();
        //
        //        vec3 rotation = transform.get_rotation_euler();
        //
        //        if (mousePos.y > 0) {
        //            rotation.x+= 1;
        //        } else if (mousePos.y < 0) {
        //            rotation.x -= 1;
        //        }
        //
        //        transform.set_rotation_euler(rotation);
        //
    }
}

}  // namespace knot

#include <knoting/camera_rotation.h>
#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <string_view>

namespace knot {
CameraRotation::CameraRotation(Engine& engine)
    : m_engine(engine), m_inputManager(engine.get_window_module().lock()->get_input_manager()) {}

void CameraRotation::on_awake() {
    m_engine.get_window_module().lock()->set_cursor_hide(m_lockState);

    using namespace components;
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }

    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto cameras = registry.view<Transform, EditorCamera, Name>();

    for (auto& cam : cameras) {
        auto goOpt = scene.get_game_object_from_handle(cam);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();

        auto rotation = transform.get_rotation_euler();

        m_roll = rotation.x;
        m_pitch = rotation.y;
        m_yaw = rotation.z;
    }
}

void CameraRotation::on_update(double m_delta_time) {
    //= CHECK VALID INPUT ==
    vec2d currentMousePos = m_inputManager.get_absolute_position();
    if (glm::isinf(currentMousePos.x) || glm::isinf(currentMousePos.y)) {
        return;
    }
    m_mouseDelta = (currentMousePos - m_lastMousePosition);

    //= 'E' TOGGLE MOUSE HIDDEN ==
    toggle_mouse_hidden();

    using namespace components;
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }

    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto cameras = registry.view<Transform, EditorCamera, Name>();

    for (auto& cam : cameras) {
        if (m_lockState == false) {
            continue;
        }
        auto goOpt = scene.get_game_object_from_handle(cam);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();
        EditorCamera& editorCamera = go.get_component<EditorCamera>();
        Name& name = go.get_component<Name>();

        m_roll = 0.0f;
        m_pitch += ((float)-m_mouseDelta.y * (float)m_mouseSensitivity.y) * (float)m_delta_time;
        m_yaw += ((float)-m_mouseDelta.x * (float)m_mouseSensitivity.x) * (float)m_delta_time;

        m_pitch = clamp(m_pitch, m_pitchClamp.x, m_pitchClamp.y);

        transform.set_rotation_euler(vec3(m_roll, m_pitch, m_yaw));
        auto pos = transform.get_rotation_euler();

        //= CALCULATE FORWARD VECTOR == // TODO STORE / CALC THESE IN TRANSFORM
        glm::vec3 look;
        look.x = cosf(radians(pos.y)) * sinf(radians(pos.z));
        look.y = sinf(radians(pos.y));
        look.z = cosf(radians(pos.y)) * cosf(radians(pos.z));
        vec3 forward = glm::normalize(look);

        //= CALCULATE LOCAL RIGHT AND UP == // TODO STORE / CALC THESE IN TRANSFORM
        vec3 right = glm::normalize(glm::cross(forward, vec3(0, 1, 0)));
        vec3 up = glm::normalize(glm::cross(right, forward));

        //= SET LOOK TARGET POSITION USING FORWARD VECTOR
        vec3 targetPos = transform.get_position() + forward;
        editorCamera.set_look_target(targetPos);

        m_lastMousePosition = currentMousePos;
    }
}
void CameraRotation::on_late_update() {}
void CameraRotation::on_destroy() {}

void CameraRotation::toggle_mouse_hidden() {
    if (m_inputManager.key_on_release(KeyCode::E)) {
        if (m_ePressed) {
            log::info("E PRESSED");
            m_lockState = !m_lockState;
            m_engine.get_window_module().lock()->set_cursor_hide(m_lockState);
        }
        m_ePressed = false;
    } else {
        m_ePressed = true;
    }
}
}  // namespace knot
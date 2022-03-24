#include <knoting/camera_rotation.h>
#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <string_view>

namespace knot {
CameraRotation::CameraRotation(Engine& engine) : m_engine(engine) {
    m_inputManager = m_engine.get_window_module().lock()->get_input_manager();
}

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

        m_pitch = rotation.x;
        m_yaw = rotation.y;
        m_roll = rotation.z;
    }
}

void CameraRotation::on_update(double m_delta_time) {
    using namespace components;

    //= CHECK VALID INPUT ==
    vec2d currentMousePos = m_inputManager->get_absolute_position();
    if (glm::isinf(currentMousePos.x) || glm::isinf(currentMousePos.y)) {
        return;
    }
    m_mouseDelta = (currentMousePos - m_lastMousePosition);

    //= 'E' TOGGLE MOUSE HIDDEN ==
    camera_key_input();

    auto cameraOpt = EditorCamera::get_active_camera();
    if (!cameraOpt)
        return;
    auto& editorCamera = cameraOpt.value().get();

    if (m_lockState == false) {
        return;
    }
    auto goOpt = GameObject::get_game_object_from_component(editorCamera);
    if (!goOpt) {
        return;
    }

    GameObject go = goOpt.value();
    Transform& transform = go.get_component<Transform>();
    Name& name = go.get_component<Name>();

    //= CAMERA ROTATION
    m_roll = 0.0f;
    m_pitch -= ((float)m_mouseDelta.y * (float)m_mouseSensitivity.y) * (float)m_delta_time;
    m_yaw -= ((float)-m_mouseDelta.x * (float)m_mouseSensitivity.x) * (float)m_delta_time;

    m_pitch = clamp(m_pitch, -89.f, 89.f);
    transform.set_rotation_euler(vec3(m_pitch, m_yaw, m_roll));

    //= CALCULATE FORWARD VECTOR == // TODO STORE / CALC THESE IN TRANSFORM
    vec3 look;
    look.x = cosf(radians(m_yaw)) * cosf(radians(m_pitch));
    look.y = sinf(radians(m_pitch));
    look.z = sinf(radians(m_yaw)) * cosf(radians(m_pitch));

    m_forward = normalize(look);

    //= CALCULATE LOCAL RIGHT AND UP == // TODO STORE / CALC THESE IN TRANSFORM
    m_right = normalize(cross(m_forward, vec3(0, 1, 0)));
    m_up = normalize(cross(m_right, m_forward));

    //= CAMERA MOVEMENT
    vec3 position = transform.get_position();
    vec3 nextPosition = position + (m_keyboardDirection * m_movementMultiplier * m_moveSpeed * (float)m_delta_time);
    transform.set_position(nextPosition);

    //= SET LOOK TARGET POSITION USING FORWARD VECTOR
    vec3 targetPos = transform.get_position() + m_forward;
    editorCamera.set_look_target(targetPos);

    m_lastMousePosition = currentMousePos;
}

void CameraRotation::on_late_update() {}

void CameraRotation::on_destroy() {}

void CameraRotation::camera_key_input() {
    if (m_inputManager->key_pressed(KeyCode::E)) {
        if (m_ePressed) {
            m_lockState = !m_lockState;
            m_engine.get_window_module().lock()->set_cursor_hide(m_lockState);
        }
        m_ePressed = false;
    } else {
        m_ePressed = true;
    }

    //= XYZ editor movement input

    vec3 movementDirectionXYZ = vec3(0);

    //    if (m_inputManager->key_pressed(KeyCode::W)) {
    //        movementDirectionXYZ += m_forward;
    //    }
    //    if (m_inputManager->key_pressed(KeyCode::S)) {
    //        movementDirectionXYZ += -m_forward;
    //    }
    //    if (m_inputManager->key_pressed(KeyCode::A)) {
    //        movementDirectionXYZ += -m_right;
    //    }
    //    if (m_inputManager->key_pressed(KeyCode::D)) {
    //        movementDirectionXYZ += m_right;
    //    }
    //    if (m_inputManager->key_pressed(KeyCode::R)) {
    //        movementDirectionXYZ += m_up;
    //    }
    //    if (m_inputManager->key_pressed(KeyCode::F)) {
    //        movementDirectionXYZ += -m_up;
    //    }

    vec3 speedTarget;
    if (m_inputManager->key_pressed(KeyCode::LeftShift)) {
        speedTarget = m_maxMovementMultiplier;
    } else {
        speedTarget = m_minMovementMultiplier;
    }
    m_movementMultiplier = lerp(m_movementMultiplier, speedTarget, .15f);

    m_keyboardDirection = movementDirectionXYZ;
}
}  // namespace knot

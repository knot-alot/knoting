#include <bgfx/bgfx.h>
#include <knoting/camera_rotation.h>
#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/forward_renderer.h>
#include <knoting/instance_mesh.h>
#include <knoting/light_data.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/scene.h>
#include <knoting/shader_program.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <knoting/subsystem.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/scene.h>
#include <stb_image.h>
#include <fstream>
#include <string_view>

namespace knot {
CameraRotation::CameraRotation(Engine& engine)
    : m_engine(engine), m_inputManager(engine.get_window_module().lock()->get_input_manager()) {}

void CameraRotation::on_awake() {
    m_engine.get_window_module().lock()->set_cursor_hide(m_lockState);
}

void CameraRotation::on_update(double m_delta_time) {
//    if (is_dirty == false) {
//        is_dirty = true;
//        m_lastMousePosition = m_inputManager.get_absolute_position();
//        return;
//    }
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
        EditorCamera& editorCamera = go.get_component<EditorCamera>();
        Name& name = go.get_component<Name>();

        vec2d currentMousePos = m_inputManager.get_absolute_position();



        if(glm::isinf(currentMousePos.x) || glm::isinf(currentMousePos.y)){
            log::error("mouse pos is infinite");
            return;
        }

        vec2d mouseDelta = (currentMousePos - m_lastMousePosition);
        mouseDelta = clamp(mouseDelta, vec2d(-700), vec2d(700));

        if (mouseDelta.x != 0 || mouseDelta.y != 0) {
            log::info("DELTA : x: {} , y: {}", mouseDelta.x, mouseDelta.y);
            auto currentRotation = transform.get_rotation_euler();

            m_yaw += ((float)-mouseDelta.x * (float)m_mouseSensitivity.x) * (float)m_delta_time;
            m_pitch += ((float)-mouseDelta.y * (float)m_mouseSensitivity.y) * (float)m_delta_time;
            m_roll = 0.0f;

            log::info("PITCH {}",m_pitch);

            m_pitch = clamp(m_pitch, m_pitchClamp.x, m_pitchClamp.y);

            //            transform.set_rotation_euler(transform.get_rotation_euler() += vec3(roll, pitch, yaw));

            //            m_yaw = radians(m_yaw);
            //            m_pitch = radians(m_pitch);
            // roll = x
            // pitch = y
            // yaw = z
//            m_pitch = clamp(m_pitch, m_pitchClamp.y,m_pitchClamp.y);

            glm::vec3 look;
            look.x = cosf(radians(m_pitch)) * sinf(radians(m_yaw));
            look.y = sinf(radians(m_pitch));
            look.z = cosf(radians(m_pitch)) * cosf(radians(m_yaw));


            vec3 forward = glm::normalize(look);

            // recalc right & up vector
            vec3 right = glm::normalize(glm::cross(forward, vec3(0, 1, 0)));
            vec3 up = glm::normalize(glm::cross(right, forward));

            vec3 targetPos = transform.get_position() + forward;
            editorCamera.set_look_target(targetPos);
        }
        m_lastMousePosition = currentMousePos;
    }
}
void CameraRotation::on_late_update() {}
void CameraRotation::on_destroy() {}
}  // namespace knot
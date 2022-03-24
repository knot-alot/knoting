#pragma once

#include <knoting/component.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>

namespace knot {
namespace components {

class EditorCamera : public Component<EditorCamera> {
   public:
    EditorCamera();
    ~EditorCamera();

    void on_awake() override;
    void on_destroy() override;

    vec3 get_look_target() { return m_lookTarget; };
    float get_fov() { return radians(m_fov); };
    float get_z_near() { return m_zNear; };
    float get_z_far() { return m_zFar; };
    float get_move_speed() { return m_moveSpeed; };

    vec3 get_up();

    void set_look_target(vec3 target) { m_lookTarget = target; };
    void set_fov(float fov) { m_fov = fov; };
    void set_z_near(float zNear) { m_zNear = zNear; };
    void set_z_far(float zFar) { m_zFar = zFar; };
    void set_move_speed(float speed) { m_moveSpeed = speed; };

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_lookTarget), CEREAL_NVP(m_fov), CEREAL_NVP(m_zNear), CEREAL_NVP(m_zFar),
                CEREAL_NVP(m_moveSpeed), CEREAL_NVP(m_moveSpeedMultiplier));
    }

    static std::optional<std::reference_wrapper<EditorCamera>> get_active_camera() { return s_activeCamera; }

    static void set_active_camera(std::optional<std::reference_wrapper<EditorCamera>> editorCamera) {
        s_activeCamera = editorCamera;
    }

   private:
    glm::vec3 m_lookTarget = vec3(0, 0, -1);

    float m_fov = 60.0f;

    float m_zNear = 0.1f;
    float m_zFar = 800.0f;

    float m_moveSpeed = 5;
    float m_moveSpeedMultiplier = 4;

    inline static std::optional<std::reference_wrapper<EditorCamera>> s_activeCamera = std::nullopt;
};

}  // namespace components
}  // namespace knot

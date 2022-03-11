#pragma once
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <memory>

namespace knot {

class Engine;
class InputManager;

}  // namespace knot

namespace knot {

class CameraRotation : public Subsystem {
   public:
    CameraRotation(Engine& engine);

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

   private:
    void toggle_mouse_hidden();

   private:
    Engine& m_engine;
    std::shared_ptr<InputManager> m_inputManager;

    vec2d m_lastMousePosition = vec2d(0.0);
    vec2d m_mouseSensitivity = vec2d(1.0);
    vec2d m_pitchClamp = vec2(-60, 60);
    vec2d m_currentMousePos = vec2d(0.0);
    vec2d m_mouseDelta = vec2d(0.0);

    double m_roll = 0;   // roll = x
    double m_pitch = 0;  // pitch = y
    double m_yaw = 0;    // yaw = z

    bool m_ePressed = true;
    bool m_lockState = true;
};

}  // namespace knot

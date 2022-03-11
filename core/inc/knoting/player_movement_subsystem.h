#pragma once
#include "subsystem.h"
#include "types.h"

namespace knot {
class Engine;
class InputManager;

}  // namespace knot
namespace knot {

class PlayerMovement : public Subsystem {
   public:
    PlayerMovement(Engine& engine);
    ~PlayerMovement();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void player_movement(double m_delta_time);
    void camera_movement(double m_delta_time);

   protected:
    Engine& m_engine;
    std::shared_ptr<InputManager> m_inManager;

   private:
    vec3 player_inputs();
    bool can_jump();

    const float m_baseSpeedMulti = 100000.0f;
    float m_speedMulti = 5;

    float jumpForce = 10000;
};

}  // namespace knot

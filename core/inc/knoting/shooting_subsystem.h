#pragma once
#include <knoting/game_object.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <deque>
namespace knot {
class Engine;
class InputManager;
}  // namespace knot
namespace knot {

class Shooting : public Subsystem {
   public:
    Shooting(Engine& engine);
    ~Shooting() = default;

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_destroy() override;

   protected:
    Engine& m_engine;
    std::shared_ptr<InputManager> m_inManager;

    std::vector<GameObject> bullets;
    float m_timepassed;
    float m_firing_rate;
    uint16_t bullSize = 0;
};

}  // namespace knot

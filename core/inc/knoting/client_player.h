#pragma once

#include <knoting/log.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>

namespace knot {
namespace components {
class ClientPlayer {
   public:
    ClientPlayer();
    ~ClientPlayer();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void set_client_num(uint16_t cliNum) { m_clientNum = cliNum; }
    uint16_t get_client_num() const { return m_clientNum; }

    void set_look_axis(vec2i lookAxis = vec2i(0)) { m_lookAxis = lookAxis; }
    vec2i get_look_axis() { return m_lookAxis; }

    void set_XZ_movement_axis(vec2i moveAxis = vec2i(0)) { m_moveAxis = moveAxis; }
    vec2i get_movement_axis() { return m_moveAxis; }

    void set_jumping_pressed(bool jump) { jumpPressed = jump; }
    bool get_jumping_pressed() const { return jumpPressed; }

    void set_is_shooting(bool shoot) { isShooting = shoot; }
    bool get_is_shooting() const { return isShooting; }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_clientNum));
    }

   protected:
    uint16_t m_clientNum = 0;

    vec2i m_lookAxis = vec2i(0);
    vec2i m_moveAxis = vec2i(0);

    bool jumpPressed = false;
    bool isShooting = false;
};
}  // namespace components
}  // namespace knot

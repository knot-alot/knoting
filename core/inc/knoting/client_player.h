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

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_clientNum));
    }

    uint16_t m_thisClientNum = 0;
    uint16_t m_clientNum = 0;

    vec2i m_lookAxis = vec2i(0);
    vec2i m_moveAxis = vec2i(0);

    bool m_jumpPressed = false;
    bool m_isShooting = false;
};
}  // namespace components
}  // namespace knot

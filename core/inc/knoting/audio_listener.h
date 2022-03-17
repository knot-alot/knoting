#pragma once

#include <knoting/components.h>
#include <knoting/types.h>
#include <fmod.hpp>

namespace knot {
namespace components {

class AudioListener {
   public:
    explicit AudioListener(float range);
    ~AudioListener() = default;

    FMOD_VECTOR* get_position();
    quat get_rotation();
    void update_velocity();
    FMOD_VECTOR get_velocity();
    FMOD_VECTOR* get_forward();
    FMOD_VECTOR* get_up();

   private:
    float m_range;
    FMOD_VECTOR m_last_pos;
    FMOD_VECTOR m_velocity;
};

}  // namespace components
}  // namespace knot
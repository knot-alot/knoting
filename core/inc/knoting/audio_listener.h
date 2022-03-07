#pragma once
#include <knoting/components.h>
#include <knoting/types.h>
#include <fmod.hpp>

namespace knot::components {
class AudioListener {
   public:
    explicit AudioListener(float range);
    ~AudioListener() = default;

    vec3 get_position() const;
    quat get_rotation() const;

   private:
    float m_range;
};

}  // namespace knot::components
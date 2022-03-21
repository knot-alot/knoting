#pragma once

#include <knoting/components.h>
#include <knoting/types.h>
#include <fmod.hpp>

namespace knot::components {

class AudioListener {
   public:
    vec3 get_position();
    quat get_rotation();
    vec3 get_forward();
    vec3 get_up();

    template <class Archive>
    void save(Archive& archive) const {}

    template <class Archive>
    void load(Archive& archive) {}

   protected:
    bool working = true;
};

}  // namespace knot::components
#pragma once

#include <knoting/components.h>
#include <knoting/types.h>
#include <fmod.hpp>

namespace knot::components {

class AudioListener {
   public:
    AudioListener() = default;
    ~AudioListener() = default;
    AudioListener(const AudioListener& other) = default;
    //    //=For ECS========
    //    void on_awake() {}
    //    void on_destroy() {}
    //    //================

    FMOD_VECTOR* get_position();
    quat get_rotation();
    FMOD_VECTOR* get_forward();
    FMOD_VECTOR* get_up();

    template <class Archive>
    void save(Archive& archive) const {}

    template <class Archive>
    void load(Archive& archive) {}

   protected:
    bool working = true;
};

}  // namespace knot::components
#pragma once

#include <knoting/components.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <fmod.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace knot {
namespace components {

class AudioSource;
class AudioListener;

}  // namespace components
}  // namespace knot

namespace knot {
class AudioSubsystem : public Subsystem {
   public:
    void on_awake() override;
    void on_destroy() override;
    void on_update(double m_delta_time) override;

    void play(components::AudioSource& source);
    void toggle(components::AudioSource& source);
    void stop(components::AudioSource& source);
    void stop();
    void update();
    void add_sound(components::AudioSource& sound);
    void update_source(components::AudioSource& source);
    void update_listener(components::AudioListener& listener);
    void set_loop(components::AudioSource& source, bool loops);

    inline static FMOD_VECTOR vec3_to_FMOD_VEC(const vec3& v) { return {v.x, v.y, v.z}; }

   protected:
    FMOD::System* m_system = nullptr;
    static constexpr int m_frequency = 20000;
    static constexpr int m_priority = 128;
    static constexpr float m_minDis = 0.0f;
    static constexpr float m_maxDis = 50000.0f;

    FMOD_RESULT m_result;
};

}  // namespace knot
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
    AudioSubsystem();
    ~AudioSubsystem() = default;

    void on_awake() override;
    void on_destroy() override;
    static int ErrorCheck(FMOD_RESULT result) {
        KNOTING_ASSERT(result == FMOD_OK);
        return 0;
    };
    void play(components::AudioSource& source);
    void toggle(components::AudioSource& source);
    void stop(components::AudioSource& source);
    void stop();
    void update();
    void add_sound(components::AudioSource& sound);
    void update_source(components::AudioSource& source);
    void update_listener(components::AudioListener& listener);
    void set_loop(components::AudioSource& source, bool loops);

    void on_update(double m_delta_time) override;

   protected:
    FMOD::System* m_system = nullptr;

    FMOD_RESULT m_result;
};

}  // namespace knot
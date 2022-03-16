#pragma once
#include <knoting/components.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <fmod.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace knot {
class AudioSource;
class AudioListener;
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
    void play(components::AudioSource* source);
    void stop(components::AudioSource* source);
    void update();
    void add_sound(components::AudioSource* sound);
    void update_source(components::AudioSource* source);
    void update_listener(components::AudioListener* listener);

    void on_update(double m_delta_time) override;

   private:
    FMOD::System* m_system = nullptr;
    FMOD::Channel* m_channel = nullptr;
    std::vector<components::AudioSource*> sounds;

    FMOD_RESULT m_result;
};

}  // namespace knot
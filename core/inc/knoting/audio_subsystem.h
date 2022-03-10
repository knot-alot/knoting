#pragma once
#include <knoting/subsystem.h>
#include <fmod.hpp>
#include <vector>

namespace knot::components {
class AudioSource;
class AudioListener;
class AudioSubsystem : public Subsystem {
   public:
    AudioSubsystem() = default;
    ~AudioSubsystem() = default;

    void on_awake() override;
    void on_destroy() override;

    void play(AudioSource* source);
    void stop(AudioSource* source);
    void update(AudioListener* listener);
    void add_sound(AudioSource* sound);
    void set_listener(AudioListener* listener);
    static AudioSubsystem& get_instance() { return *m_instance; };

   private:
    static AudioSubsystem* m_instance;
    FMOD::System* m_system;
    FMOD::Channel* m_channel;
    std::vector<AudioSource*> sounds;
    AudioListener* m_listener;
};

}  // namespace knot::components
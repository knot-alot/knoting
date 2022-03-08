#pragma once
#include <knoting/subsystem.h>
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

    void play_sounds();
    void update();
    void add_sound(AudioSource* sound);
    void set_listener(AudioListener* listener);

   private:
    std::vector<AudioSource*> sounds;
    AudioListener* m_listener;
};

}  // namespace knot::components
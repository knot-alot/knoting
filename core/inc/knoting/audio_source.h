#pragma once
#include <knoting/asset.h>
#include <knoting/audio_listener.h>
#include <knoting/components.h>
#include <knoting/types.h>
#include <fmod.hpp>
#include <string>

namespace knot::components {
class AudioSource : public Asset {
   public:
    AudioSource();
    ~AudioSource();

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void set_path(std::string& path) { m_path = path; }
    std::string get_path() { return m_path; }
    void set_loop(bool loop) { m_loops = loop; }
    bool get_loop() { return m_loops; }
    FMOD_VECTOR get_position() const;

    bool get_is_playing() { return m_is_playing; }
    void set_is_playing(bool is_playing) { m_is_playing = is_playing; }
    FMOD::Sound* get_sound() { return m_sound; }

   private:
    FMOD::Sound* m_sound{};
    std::string m_path;
    bool m_loops{};
    bool m_is_playing{};
};
}  // namespace knot::components
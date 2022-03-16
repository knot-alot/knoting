#pragma once

#include <knoting/asset.h>
#include <knoting/audio_listener.h>
#include <knoting/components.h>
#include <knoting/types.h>

#include <filesystem>
#include <fmod.hpp>
#include <string>

namespace knot {
class Engine;
}

namespace knot::components {

class AudioSource : public Asset {
   public:
    AudioSource(const std::filesystem::path& path);
    ~AudioSource();

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void set_path(const std::filesystem::path& path) {
        m_path = path;  // TODO: Need to destroy the current FMOD::Sound and recreate
    }
    const std::filesystem::path& get_path() { return m_path; }

    void set_loop(bool loop) { m_loops = loop; }
    bool get_loop() { return m_loops; }
    FMOD_VECTOR* get_position() const;

    bool& get_is_playing() { return m_isPlaying; }
    void set_is_playing(bool isPlaying) { m_isPlaying = isPlaying; }
    FMOD::Sound*& get_sound() { return m_sound; }

   private:
    FMOD::Sound* m_sound;
    std::filesystem::path m_path;
    bool m_loops = true;
    bool m_isPlaying = false;
    bool isLoaded = false;
};

}  // namespace knot::components
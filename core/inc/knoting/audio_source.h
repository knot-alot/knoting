#pragma once

#include <knoting/asset.h>
#include <knoting/components.h>
#include <knoting/types.h>

#include <filesystem>
#include <fmod.hpp>
#include <string>

namespace knot {
class Engine;
class AudioSubsystem;
}  // namespace knot

namespace knot::components {

class AudioSource : public Asset {
   public:
    AudioSource(const std::filesystem::path& path = {}, bool loops = false);
    ~AudioSource();

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void set_path(const std::filesystem::path& path) { m_path = path; }
    const std::filesystem::path& get_path() { return m_path; }

    bool get_loops() { return m_loops; }
    FMOD_VECTOR* get_position() const;

    bool& get_is_playing() { return m_isPlaying; }
    void set_is_playing(bool isPlaying) { m_isPlaying = isPlaying; }
    FMOD::Sound*& get_sound() { return m_sound; }

    template <class Archive>
    void save(Archive& archive) const {
        std::string path = m_path.string();
        archive(CEREAL_NVP(path), CEREAL_NVP(m_isPaused), CEREAL_NVP(m_loops), CEREAL_NVP(m_isPlaying));
    }

    template <class Archive>
    void load(Archive& archive) {
        std::string path = "";
        archive(CEREAL_NVP(path), CEREAL_NVP(m_isPaused), CEREAL_NVP(m_loops), CEREAL_NVP(m_isPlaying));
        m_path = path;
        on_awake();
    }

   private:
    friend class AudioSubsystem;

    FMOD::Sound* m_sound;
    FMOD::Channel* m_channel = nullptr;
    std::filesystem::path m_path;

    bool m_isPaused = false;
    bool m_loops = true;
    bool m_isPlaying = false;
};

}  // namespace knot::components
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

class AudioSource : public Asset, public Component<AudioSource> {
   public:
    AudioSource(const std::string& path = {}, bool loops = false);

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override {}
    //=================

    vec3 get_position() const;

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

    FMOD::Channel* m_channel = nullptr;
    bool m_isPaused = false;
    bool m_isPlaying = false;
    std::filesystem::path m_path;
    std::filesystem::path m_fullPath;
    bool m_loops = true;

   protected:
    friend class AudioSubsystem;

    FMOD::Sound* m_sound;
};

}  // namespace knot::components

#pragma once
#include <knoting/asset.h>
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

    void play();
    void stop();
    void update();
    void set_path(std::string& path) { m_path = path; };
    void set_loop(bool loop) { m_loops = loop; }
    vec3 get_position() const;

   private:
    static AudioSource* m_instance;
    FMOD::System* m_system{};
    FMOD::Channel* m_channel{};
    FMOD::Sound* m_sound{};
    std::string m_path;
    bool m_loops{};
    bool m_is_playing{};
};
}  // namespace knot::components
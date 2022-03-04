#pragma once
#include <knoting/asset.h>
#include <knoting/types.h>
#include <fmod.hpp>
#include <string>


namespace knot {
namespace components {
class AudioSource : public Asset {
   public:
    AudioSource();
    AudioSource(const std::string& path);
    ~AudioSource() ;

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    static void startup();
    void play();
    void stop();
    void update();

   private:
    static AudioSource* m_instance;
    FMOD::System* m_system;
    FMOD::Channel* m_channel;
    FMOD::Sound* m_sound;
    std::string m_path;
    bool m_loops;
    bool is_playing;
};
}  // namespace components
}  // namespace knot
#include "knoting/audio_source.h"

namespace knot::components {

FMOD_RESULT m_result;

AudioSource* AudioSource::m_instance = nullptr;

AudioSource::AudioSource() : Asset{AssetType::Audio, ""} {
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    m_result = m_system->init(32, FMOD_INIT_NORMAL, extra_driver_data);
    m_channel = nullptr;

    // temp
    std::string test_sound_file_path = "..//Assets/Audio/";
    test_sound_file_path.append("drumloop.wav");
    m_result = m_system->createSound(test_sound_file_path.c_str(), FMOD_2D, nullptr, &m_sound);
    m_result = m_sound->setMode(FMOD_LOOP_OFF);
    m_sound->setDefaults(12000, 128);
}

AudioSource::AudioSource(const std::string& path, bool loop) : Asset{AssetType::Audio, path} {
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    m_result = m_system->init(32, FMOD_INIT_NORMAL, extra_driver_data);
    m_channel = nullptr;

    // temp
    m_result = m_system->createSound(path.c_str(), FMOD_2D, nullptr, &m_sound);
    if (!loop) {
        m_result = m_sound->setMode(FMOD_LOOP_OFF);
    } else {
        m_result = m_sound->setMode(FMOD_LOOP_NORMAL);
    }
    m_loops = loop;
    m_sound->setDefaults(12000, 128);
}

AudioSource::~AudioSource() = default;

void AudioSource::play() {
    m_result = m_channel->isPlaying(&is_playing);

    if (!is_playing) {
        m_result = m_system->playSound(m_sound, nullptr, false, &m_channel);
    }
}

void AudioSource::stop() {}

void AudioSource::update() {
    unsigned int pos = 0;
    unsigned int len_ms = 0;
    bool playing = false;
    bool paused = false;

    m_result = m_system->update();

    if (m_channel) {
        FMOD::Sound* current_sound = nullptr;
        m_result = m_channel->isPlaying(&playing);
        m_result = m_channel->getPaused(&paused);
        m_result = m_channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
        m_channel->getCurrentSound(&current_sound);

        if (current_sound) {
            m_result = current_sound->getLength(&len_ms, FMOD_TIMEUNIT_MS);
        }
    }
}
void AudioSource::on_awake() {
    m_instance = new AudioSource();
}
void AudioSource::on_destroy() {
    m_result = m_sound->release();
    m_result = m_system->close();
    m_result = m_system->release();
    delete m_instance;
}

}  // namespace knot::components
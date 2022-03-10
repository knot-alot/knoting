#include "knoting/audio_subsystem.h"
#include "knoting/audio_listener.h"
#include "knoting/audio_source.h"

namespace knot::components {
FMOD_RESULT m_result;

AudioSubsystem* AudioSubsystem::m_instance = nullptr;

void AudioSubsystem::on_awake() {
    m_instance = new AudioSubsystem();
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    m_result = m_system->init(512, FMOD_INIT_NORMAL, extra_driver_data);
    m_result = m_system->set3DSettings(1.0f, 1.0f, 1.0f);
    m_channel = nullptr;
}

void AudioSubsystem::on_destroy() {
    // m_result = m_sound->release();
    m_result = m_system->close();
    m_result = m_system->release();
    delete m_instance;
}

void AudioSubsystem::play(AudioSource* source) {
    source->set_is_playing(false);
    m_result = m_channel->isPlaying(reinterpret_cast<bool*>(source->get_is_playing()));

    if (!source->get_is_playing()) {
        m_result = m_system->playSound(source->get_sound(), nullptr, false, &m_channel);

        FMOD_VECTOR pos = source->get_position();
        FMOD_VECTOR vel = {0, 0, 0};
        m_result = m_channel->set3DAttributes(&pos, &vel);
        source->set_is_playing(true);
    }
}

void AudioSubsystem::stop(AudioSource* source) {
    if (m_channel)
        m_channel->stop();
}

void AudioSubsystem::update(AudioListener* listener) {
    unsigned int pos = 0;
    unsigned int len_ms = 0;
    bool playing = false;
    bool paused = false;

    m_result = m_system->update();

    FMOD_VECTOR forward = {0, 0, listener->get_rotation().z};
    FMOD_VECTOR up = {0.0f, listener->get_rotation().y, 0.0f};
    FMOD_VECTOR listener_pos = listener->get_position();
    FMOD_VECTOR vel = listener->get_velocity();

    m_result = m_system->set3DListenerAttributes(0, &listener_pos, &vel, &forward, &up);

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

void AudioSubsystem::add_sound(AudioSource* source) {
    m_result = m_system->createSound(source->get_path().c_str(), FMOD_3D, nullptr,
                                     reinterpret_cast<FMOD::Sound**>(source->get_sound()));
    m_result = source->get_sound()->set3DMinMaxDistance(0.5f, 5000.0f);
    if (!source->get_loop()) {
        m_result = source->get_sound()->setMode(FMOD_LOOP_OFF);
    } else {
        m_result = source->get_sound()->setMode(FMOD_LOOP_NORMAL);
    }
    source->get_sound()->setDefaults(12000, 128);
    sounds.emplace_back(source);
    // add sort vector by distance
}

void AudioSubsystem::set_listener(AudioListener* listener) {
    m_listener = listener;
}

}  // namespace knot::components

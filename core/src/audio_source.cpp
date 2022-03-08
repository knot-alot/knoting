#include "knoting/audio_source.h"
#include <knoting/scene.h>

namespace knot::components {

FMOD_RESULT m_result;

AudioSource* AudioSource::m_instance = nullptr;

AudioSource::AudioSource() : Asset{AssetType::Audio, m_path} {}

AudioSource::~AudioSource() = default;

void AudioSource::play() {
    m_is_playing = false;
    m_result = m_channel->isPlaying(&m_is_playing);

    if (!m_is_playing) {
        m_result = m_system->playSound(m_sound, nullptr, false, &m_channel);

        FMOD_VECTOR pos = this->get_position();
        FMOD_VECTOR vel = {0, 0, 0};
        m_result = m_channel->set3DAttributes(&pos, &vel);
        m_is_playing = true;
    }
}

void AudioSource::stop() {
    if (m_channel)
        m_channel->stop();
}

void AudioSource::update(AudioListener* listener) {
    // need to do something about getting the listener position and velocity

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
void AudioSource::on_awake() {
    m_instance = new AudioSource();
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    m_result = m_system->init(32, FMOD_INIT_NORMAL, extra_driver_data);
    m_result = m_system->set3DSettings(1.0f, 1.0f, 1.0f);
    m_channel = nullptr;

    m_result = m_system->createSound(m_path.c_str(), FMOD_3D, nullptr, &m_sound);
    m_result = m_sound->set3DMinMaxDistance(0.5f, 5000.0f);
    if (!m_loops) {
        m_result = m_sound->setMode(FMOD_LOOP_OFF);
    } else {
        m_result = m_sound->setMode(FMOD_LOOP_NORMAL);
    }

    m_sound->setDefaults(12000, 128);
}
void AudioSource::on_destroy() {
    m_result = m_sound->release();
    m_result = m_system->close();
    m_result = m_system->release();
    delete m_instance;
}
FMOD_VECTOR AudioSource::get_position() const {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            vec3 position = goOpt->get_component<components::Transform>().get_position();
            FMOD_VECTOR pos = {position.x, position.y, position.z};
            return pos;
        }
    }
}
}  // namespace knot::components
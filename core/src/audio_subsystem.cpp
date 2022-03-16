#include "knoting/audio_subsystem.h"

namespace knot {

AudioSubsystem::AudioSubsystem() = default;

void AudioSubsystem::on_awake() {
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    ErrorCheck(m_result);
    m_result = m_system->init(512, FMOD_INIT_NORMAL, extra_driver_data);
    ErrorCheck(m_result);
    //  m_result = m_system->set3DSettings(1.0f, 1.0f, 1.0f);
    m_channel = nullptr;
    log::debug("AudioSubsystem created!");
}

void AudioSubsystem::on_destroy() {
    m_result = m_system->close();
    ErrorCheck(m_result);
    m_result = m_system->release();
    ErrorCheck(m_result);
}

void AudioSubsystem::play(components::AudioSource* source) {
    source->set_is_playing(false);
    if (m_channel) {
        m_result = m_channel->isPlaying(&source->get_is_playing());
        ErrorCheck(m_result);
    }

    if (!source->get_is_playing()) {
        m_result = m_system->playSound(source->get_sound(), nullptr, false, &m_channel);
        ErrorCheck(m_result);

        //        FMOD_VECTOR pos = source->get_position();
        FMOD_VECTOR pos = {0, 0, 0};
        FMOD_VECTOR vel = {0, 0, 0};
        // m_result = m_channel->set3DAttributes(&pos, &vel);
        source->set_is_playing(true);
    }
}

void AudioSubsystem::stop(components::AudioSource* source) {
    if (m_channel)
        m_channel->stop();
}

void AudioSubsystem::update(components::AudioListener& listener) {
    unsigned int pos = 0;
    unsigned int len_ms = 0;
    bool playing = false;
    bool paused = false;

    m_result = m_system->update();
    ErrorCheck(m_result);

    //    FMOD_VECTOR forward = {0, 0, listener.get_rotation().z};
    //    FMOD_VECTOR up = {0.0f, listener.get_rotation().y, 0.0f};
    //    FMOD_VECTOR listener_pos = listener.get_position();
    //    FMOD_VECTOR vel = listener.get_velocity();
    FMOD_VECTOR forward = {0, 0, 1};
    FMOD_VECTOR up = {0.0f, 1, 0.0f};
    FMOD_VECTOR listener_pos = {0, 0, 0};
    FMOD_VECTOR vel = {0, 0, 0};

    // m_result = m_system->set3DListenerAttributes(0, &listener_pos, &vel, &forward, &up);

    if (m_channel) {
        FMOD::Sound* current_sound = nullptr;
        m_result = m_channel->isPlaying(&playing);
        ErrorCheck(m_result);
        m_result = m_channel->getPaused(&paused);
        ErrorCheck(m_result);
        m_result = m_channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
        ErrorCheck(m_result);
        m_channel->getCurrentSound(&current_sound);

        if (current_sound) {
            m_result = current_sound->getLength(&len_ms, FMOD_TIMEUNIT_MS);
            ErrorCheck(m_result);
        }
    }
}

void AudioSubsystem::add_sound(components::AudioSource* source) {
    log::info("TRYING TO LOAD AUDIO FILE {}", source->get_path().string());
    m_result = m_system->createSound(source->get_path().string().c_str(), FMOD_2D, nullptr, &source->get_sound());
    ErrorCheck(m_result);
    // m_result = source->get_sound()->set3DMinMaxDistance(0.0f, 5000.0f);
    if (!source->get_loop()) {
        m_result = source->get_sound()->setMode(FMOD_LOOP_OFF);
        ErrorCheck(m_result);
    } else {
        m_result = source->get_sound()->setMode(FMOD_LOOP_NORMAL);
        ErrorCheck(m_result);
    }
    source->get_sound()->setDefaults(12000, 128);
    // sounds.emplace_back(source);
}
void AudioSubsystem::on_update(double m_delta_time) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto sources = registry.view<components::AudioSource>();

    for (auto sourceHandle : sources) {
        auto sourceOpt = scene.get_game_object_from_handle(sourceHandle);
        if (!sourceOpt) {
            continue;
        }
        auto sourceGO = sourceOpt.value();

        auto& source = sourceGO.get_component<components::AudioSource>();
        this->play(&source);
    }

    auto listeners = registry.view<components::AudioListener>();
    for (auto listenerHandle : listeners) {
        auto listenerOpt = scene.get_game_object_from_handle(listenerHandle);
        if (!listenerOpt) {
            continue;
        }
        auto listenerGO = listenerOpt.value();

        auto& listener = listenerGO.get_component<components::AudioListener>();
        this->update(listener);
    }
}

}  // namespace knot

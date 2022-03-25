#include <knoting/audio_subsystem.h>

#include <knoting/audio_source.h>

namespace knot {

void AudioSubsystem::on_awake() {
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    m_result = m_system->init(512, FMOD_INIT_NORMAL, extra_driver_data);
    m_result = m_system->set3DSettings(1.0f, 1.0f, 1.0f);
    log::debug("AudioSubsystem created!");
}

void AudioSubsystem::on_destroy() {
    m_result = m_system->close();
    m_result = m_system->release();
}

void AudioSubsystem::play(components::AudioSource& source) {
    if (source.m_channel) {
        m_result = source.m_channel->isPlaying(&source.m_isPlaying);
    }

    if (!source.m_isPlaying) {
        m_result = m_system->playSound(source.get_sound(), nullptr, false, &source.m_channel);
        update_source(source);
        source.m_isPlaying = true;
    }
}

void AudioSubsystem::toggle(components::AudioSource& source) {
    if (!source.m_channel)
        return;

    source.m_isPaused = !source.m_isPaused;
    source.m_channel->setPaused(source.m_isPaused);
}

void AudioSubsystem::stop(components::AudioSource& source) {
    if (!source.m_channel)
        return;

    source.m_isPlaying = false;
    source.m_channel->stop();
    source.m_channel = nullptr;
}

void AudioSubsystem::stop() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    auto scene = sceneOpt.value();
    auto sources = scene.get().get_registry().view<components::AudioSource>();

    for (entt::entity sourceHandle : sources) {
        auto goOpt = scene.get().get_game_object_from_handle(sourceHandle);
        if (!goOpt)
            continue;

        auto go = goOpt.value();
        components::AudioSource& source = go.get_component<components::AudioSource>();

        stop(source);
    }
}

void AudioSubsystem::update() {
    unsigned int pos = 0;
    unsigned int len_ms = 0;
    bool playing = false;
    bool paused = false;

    m_result = m_system->update();

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    auto scene = sceneOpt.value();
    auto sources = scene.get().get_registry().view<components::AudioSource>();

    for (auto sourceHandle : sources) {
        auto goOpt = scene.get().get_game_object_from_handle(sourceHandle);
        if (!goOpt)
            continue;

        auto go = goOpt.value();
        components::AudioSource& source = go.get_component<components::AudioSource>();

        if (source.m_channel) {
            FMOD::Sound* current_sound = nullptr;
            m_result = source.m_channel->isPlaying(&playing);

            if (m_result == FMOD_ERR_INVALID_HANDLE) {
                source.m_channel = nullptr;
                continue;
            }

            m_result = source.m_channel->getPaused(&paused);
            m_result = source.m_channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
            source.m_channel->getCurrentSound(&current_sound);

            if (current_sound) {
                m_result = current_sound->getLength(&len_ms, FMOD_TIMEUNIT_MS);
            }
        }
    }
}

void AudioSubsystem::set_loop(components::AudioSource& source, bool loops) {
    source.m_loops = loops;

    m_result = source.get_sound()->setMode(source.m_loops ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

void AudioSubsystem::add_sound(components::AudioSource& source) {
    log::warn("TRYING TO LOAD AUDIO FILE {}", source.m_path.string());
    m_result =
        m_system->createSound(source.m_fullPath.string().c_str(), FMOD_3D_LINEARROLLOFF, nullptr, &source.get_sound());

    m_result = source.get_sound()->set3DMinMaxDistance(m_minDis, m_maxDis);
    if (!source.get_sound()) {
        log::error("Did NOT LOAD!");
    }
    set_loop(source, source.m_loops);
    source.get_sound()->setDefaults(m_frequency, m_priority);
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
        this->update_source(source);
    }

    auto listeners = registry.view<components::AudioListener>();
    for (auto listenerHandle : listeners) {
        auto listenerOpt = scene.get_game_object_from_handle(listenerHandle);
        if (!listenerOpt) {
            continue;
        }
        auto listenerGO = listenerOpt.value();

        auto& listener = listenerGO.get_component<components::AudioListener>();
        this->update_listener(listener);
    }
}

void AudioSubsystem::update_source(components::AudioSource& source) {
    FMOD_VECTOR pos = vec3_to_FMOD_VEC(source.get_position());
    FMOD_VECTOR vel = {0, 0, 0};

    if (source.m_channel) {
        m_result = source.m_channel->set3DAttributes(&pos, &vel);
    }

    update();
}

void AudioSubsystem::update_listener(components::AudioListener& listener) {
    FMOD_VECTOR forward = vec3_to_FMOD_VEC(listener.get_forward());
    FMOD_VECTOR up = vec3_to_FMOD_VEC(listener.get_up());

    FMOD_VECTOR listenerPos = vec3_to_FMOD_VEC(listener.get_position());

    FMOD_VECTOR vel = {0, 0, 0};

    m_result = m_system->set3DListenerAttributes(0, &listenerPos, &vel, &forward, &up);
    update();
}

}  // namespace knot

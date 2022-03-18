#include <knoting/audio_subsystem.h>

#include <knoting/audio_source.h>

namespace knot {

AudioSubsystem::AudioSubsystem() = default;

void AudioSubsystem::on_awake() {
    void* extra_driver_data = nullptr;
    m_result = System_Create(&m_system);
    ErrorCheck(m_result);
    m_result = m_system->init(512, FMOD_INIT_NORMAL, extra_driver_data);
    ErrorCheck(m_result);
    m_result = m_system->set3DSettings(1.0f, 1.0f, 1.0f);
    ErrorCheck(m_result);
    log::debug("AudioSubsystem created!");
}

void AudioSubsystem::on_destroy() {
    m_result = m_system->close();
    ErrorCheck(m_result);
    m_result = m_system->release();
    ErrorCheck(m_result);
}

void AudioSubsystem::play(components::AudioSource& source) {
    source.set_playing(false);
    if (source.get_channel()) {
        m_result = source.get_channel()->isPlaying(&source.get_playing());
        ErrorCheck(m_result);
    }

    if (!source.get_playing()) {
        m_result = m_system->playSound(source.get_sound(), nullptr, false, &source.get_channel());
        ErrorCheck(m_result);

        update_source(source);

        source.set_playing(true);
    }
}

void AudioSubsystem::toggle(components::AudioSource& source) {
    if (!source.get_channel())
        return;

    source.get_paused() = !source.get_paused();
    source.get_channel()->setPaused(source.get_paused());
}

void AudioSubsystem::stop(components::AudioSource& source) {
    if (!source.get_channel())
        return;

    source.set_playing(false);
    source.get_channel()->setPaused(true);
    source.set_channel(nullptr);
}

void AudioSubsystem::stop() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    auto scene = sceneOpt.value();
    auto view = scene.get().get_registry().view<components::AudioSource>();

    for (entt::entity entt : view) {
        auto goOpt = scene.get().get_game_object_from_handle(entt);
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
    ErrorCheck(m_result);

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    auto scene = sceneOpt.value();
    auto view = scene.get().get_registry().view<components::AudioSource>();

    for (entt::entity entt : view) {
        auto goOpt = scene.get().get_game_object_from_handle(entt);
        if (!goOpt)
            continue;

        auto go = goOpt.value();
        components::AudioSource& source = go.get_component<components::AudioSource>();

        if (source.get_channel()) {
            FMOD::Sound* current_sound = nullptr;
            m_result = source.get_channel()->isPlaying(&playing);

            if (m_result == FMOD_ERR_INVALID_HANDLE) {
                source.set_channel(nullptr);
                continue;
            }

            m_result = source.get_channel()->getPaused(&paused);
            ErrorCheck(m_result);
            m_result = source.get_channel()->getPosition(&pos, FMOD_TIMEUNIT_MS);
            ErrorCheck(m_result);
            source.get_channel()->getCurrentSound(&current_sound);

            if (current_sound) {
                m_result = current_sound->getLength(&len_ms, FMOD_TIMEUNIT_MS);
                ErrorCheck(m_result);
            }
        }
    }
}

void AudioSubsystem::set_loop(components::AudioSource& source, bool loops) {
    source.set_loops(loops);

    m_result = source.get_sound()->setMode(source.get_loops() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    ErrorCheck(m_result);
}

void AudioSubsystem::add_sound(components::AudioSource& source) {
    log::info("TRYING TO LOAD AUDIO FILE {}", source.get_path().string());
    m_result =
        m_system->createSound(source.get_path().string().c_str(), FMOD_3D_LINEARROLLOFF, nullptr, &source.get_sound());
    ErrorCheck(m_result);

    m_result = source.get_sound()->set3DMinMaxDistance(0.0f, 50000.0f);
    ErrorCheck(m_result);
    set_loop(source, source.get_loops());
    source.get_sound()->setDefaults(12000, 128);
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
    FMOD_VECTOR* pos = source.get_position();
    FMOD_VECTOR vel = {0, 0, 0};

    if (source.get_channel()) {
        m_result = source.get_channel()->set3DAttributes(pos, &vel);
        ErrorCheck(m_result);
    }

    update();
}

void AudioSubsystem::update_listener(components::AudioListener& listener) {
    FMOD_VECTOR forward = {listener.get_forward()->x, listener.get_forward()->y, listener.get_forward()->z};
    FMOD_VECTOR up = {listener.get_up()->x, listener.get_up()->y, listener.get_up()->z};

    FMOD_VECTOR* listenerPos = listener.get_position();

    FMOD_VECTOR vel = {0, 0, 0};

    m_result = m_system->set3DListenerAttributes(0, listenerPos, &vel, &forward, &up);
    ErrorCheck(m_result);
    update();
}

}  // namespace knot

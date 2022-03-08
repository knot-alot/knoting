#include "knoting/audio_subsystem.h"
#include "knoting/audio_listener.h"
namespace knot::components {

void AudioSubsystem::on_awake() {
    // get all sounds?
}

void AudioSubsystem::on_destroy() {
    // for each sound, call stop
}

void AudioSubsystem::play_sounds() {
    // for each sound in range, play sound
}

void AudioSubsystem::update() {
    // update listener transforms. Then, for each sound in range, call update
    m_listener->update_velocity();
}

void AudioSubsystem::add_sound(AudioSource* sound) {
    sounds.emplace_back(sound);
    // add sort vector by distance
}

void AudioSubsystem::set_listener(AudioListener* listener) {
    m_listener = listener;
}

}  // namespace knot::components

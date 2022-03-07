#include "knoting/audio_subsystem.h"

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
    // for each sound in range, call update
}

void AudioSubsystem::add_sound(AudioSource* sound) {
    sounds.emplace_back(sound);
    // add sort vector by distance
}

}  // namespace knot::components

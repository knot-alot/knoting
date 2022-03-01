#include "untie.h"
#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <iostream>

#include <cereal/archives/json.hpp>

#include <iostream>

namespace knot {
Scene scene;
Untie::Untie() {
    Scene::set_active_scene(scene);
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();
    {
        std::string filename("serialScene.json");
        std::filesystem::path path = AssetManager().get_resources_path().append(filename);
        std::fstream serializedSceneStream(path);

        auto start = std::chrono::steady_clock::now();
        serializedSceneStream.open(filename, std::ios_base::in);
        if (serializedSceneStream) {
            scene.load_scene_from_stream(serializedSceneStream);
        }
        serializedSceneStream.close();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        log::debug(elapsed_seconds.count());
    }
}

void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot

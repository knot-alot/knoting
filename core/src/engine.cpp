
#include <knoting/engine.h>

namespace knot {

Engine::Engine() {
    m_windowModule = std::make_shared<knot::Window>(m_windowWidth, m_windowHeight, m_windowTitle);

    m_engineModules.emplace_back(m_windowModule);

    for (auto& module : m_engineModules) {
        module->on_awake();
    }
}

void Engine::update_modules() {
    for (auto& module : m_engineModules) {
        module->on_update(0);
    }

    m_windowModule->draw();

    for (auto& module : m_engineModules) {
        module->on_late_update();
    }
}

Engine::~Engine() {
    for (auto& module : m_engineModules) {
        module->on_destroy();
    }
}
bool Engine::is_open() {
    return m_windowModule->is_open();
}

}  // namespace knot
#include <bx/math.h>
#include <knoting/engine.h>

namespace knot {

Engine::Engine() {
    m_windowModule = std::make_shared<knot::Window>(m_windowWidth, m_windowHeight, m_windowTitle, *this);
    m_forwardRenderModule = std::make_shared<knot::ForwardRenderer>(*this);
    m_physicsModule = std::make_shared<knot::Physics>(*this);

    // order dependent
    m_engineModules.emplace_back(m_windowModule);
    m_engineModules.emplace_back(m_forwardRenderModule);
    m_engineModules.emplace_back(m_physicsModule);

    for (auto& module : m_engineModules) {
        module->on_awake();
    }
}

void Engine::update_modules() {
    for (auto& module : m_engineModules) {
        module->on_update(m_windowModule->get_delta_time());
    }

    // TODO move into functions when functionality exists

    // CORE SYSTEM ORDER

    // PHYSICS
    // 1) SYSTEM : Physics / update all dynamic transforms

    // SCRIPTS
    // 1) SYSTEM : Editor Camera movement

    // PBR RENDERING
    // 1) SYSTEM : Shadow pass
    // 2) SYSTEM : Update Active Camera (Runtime / Editor)
    // 3) SYSTEM : Depth Pass (No Transparent Objects)
    // 4) SYSTEM : PBR Render Pass
    // 5) SYSTEM : Skybox Render
    // 6) SYSTEM : Sorted Transparent Render Pass
    // 7) SYSTEM : Post Processing Stack

    m_physicsModule->update_info_to_transform();

    //    m_forwardRenderModule->on_render();
    m_forwardRenderModule->render_pbr();
    m_forwardRenderModule->on_post_render();

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

#include <bx/math.h>
#include <knoting/engine.h>

namespace knot {

Engine::Engine() {
    InitializeYojimbo();
    set_active_engine(*this);

    m_framebufferManager = std::make_shared<knot::FramebufferManager>(*this);
    m_windowModule = std::make_shared<knot::Window>(m_windowWidth, m_windowHeight, m_windowTitle, *this);
    m_forwardRenderModule = std::make_shared<knot::ForwardRenderer>(*this);
    m_physicsModule = std::make_shared<knot::Physics>(*this);
    m_cameraRotationModule = std::make_shared<knot::CameraRotation>(*this);
    m_assetManager = std::make_shared<knot::AssetManager>();
    m_scriptingModule = std::make_shared<knot::Scripting>(*this);
    m_audioModule = std::make_shared<knot::AudioSubsystem>();
    if (!isClient) {
        m_serverModule = std::make_shared<knot::NetworkedServer>(*this);
    }
    m_clientModule = std::make_shared<knot::NetworkedClient>(*this);

    //  order dependent
    m_engineModules.emplace_back(m_framebufferManager);
    m_engineModules.emplace_back(m_windowModule);
    m_engineModules.emplace_back(m_assetManager);
    m_engineModules.emplace_back(m_cameraRotationModule);
    m_engineModules.emplace_back(m_forwardRenderModule);
    m_engineModules.emplace_back(m_physicsModule);
    m_engineModules.emplace_back(m_audioModule);
    if (!isClient) {
        m_engineModules.emplace_back(m_serverModule);
    }
    m_engineModules.emplace_back(m_scriptingModule);
    m_engineModules.emplace_back(m_clientModule);

    for (auto& module : m_engineModules) {
        module->on_awake();
    }
}

void Engine::update_modules() {
    m_windowModule->calculate_delta_time();
    auto deltaTime = m_windowModule->get_delta_time();
    for (auto& module : m_engineModules) {
        module->on_update(deltaTime);
        module->on_fixed_update();
    }
    // TODO move into functions when functionality exists

    // CORE SYSTEM ORDER

    // PHYSICS
    // 1) SYSTEM : Physics / update all dynamic transforms

    // SCRIPTS
    // 1) SYSTEM : Editor Camera movement

    // RENDERING
    // 1) SYSTEM : Shadow pass
    // 2) SYSTEM : Update Active Camera (Runtime / Editor)
    // 3) SYSTEM : Depth Pass (No Transparent Objects)
    // 4) SYSTEM : Color Render Pass
    // 5) SYSTEM : Skybox Render
    // 6) SYSTEM : Sorted Transparent Render Pass
    // 7) SYSTEM : Post Processing Stack

    for (auto& module : m_engineModules) {
        module->on_late_update();
    }
    swap_frame();
}

Engine::~Engine() {
    log::info("engine on destroy");
    for (auto& module : m_engineModules) {
        module->on_destroy();
    }
    ShutdownYojimbo();
}

bool Engine::is_open() {
    return m_windowModule->is_open();
}
void Engine::add_subsystem(std::shared_ptr<Subsystem> subsystem) {
    m_engineModules.emplace_back(subsystem);
    subsystem->on_awake();
}
void Engine::swap_frame() {
    bgfx::frame();
}

std::optional<std::reference_wrapper<Engine>> Engine::get_active_engine() {
    return s_activeEngine;
}

void Engine::set_active_engine(std::optional<std::reference_wrapper<Engine>> engine) {
    s_activeEngine = std::ref(engine);
}
void Engine::reset_physics_module() {
    m_physicsModule.reset();
    m_physicsModule = std::make_shared<knot::Physics>(*this);
    m_physicsModule->on_awake();
}

}  // namespace knot

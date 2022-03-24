#include <knoting/debug_physics_widget.h>
#include <knoting/scene.h>
#include <knoting/engine.h>

knot::DebugPhysics::DebugPhysics(const std::string& name) : Widget(name) {
}

knot::DebugPhysics::~DebugPhysics() {
}
void knot::DebugPhysics::on_widget_render() {
    using namespace components;
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();

    auto engineOpt = Engine::get_active_engine();
    if(!engineOpt){
        return;
    }
    Engine& m_engine = engineOpt.value();

    entt::registry& registry = scene.get_registry();

    mat4 proj;
    glm::mat4 view;


    auto windowSize = m_engine.get_window_module().lock()->get_window_size();

    auto cameras = registry.view<Transform, EditorCamera, Name>();

    for (auto& cam : cameras) {
        auto goOpt = scene.get_game_object_from_handle(cam);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();
        EditorCamera& editorCamera = go.get_component<EditorCamera>();
        Name& name = go.get_component<Name>();

        const glm::vec3 pos = transform.get_position();
        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        view = glm::lookAt(pos, lookTarget, up);
        proj = glm::perspective(fovY, aspectRatio, zNear, zFar);
    }
//    ImGui::Begin("DebugPhysics"
//                     , NULL
//                     , ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs
//        );
//    draw_list = ImGui::GetWindowDrawList();
//    draw_list->AddLine(ImVec2(0,0),ImVec2(100,100), 0xFFFFFFFF, 2.0f);
//    ImGui::End();
}
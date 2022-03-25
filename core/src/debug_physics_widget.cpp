#include <knoting/debug_physics_widget.h>
#include <knoting/engine.h>
#include <knoting/game_object.h>
#include <knoting/scene.h>

knot::DebugPhysics::DebugPhysics(const std::string& name) : Widget(name) {}

knot::DebugPhysics::~DebugPhysics() {}

static const float identityMatrix[16] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                                         0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

ImVec2 worldToPos(const glm::vec4 worldPos,
                  const glm::mat4 mvp,
                  ImVec2 position = ImVec2(0, 0),
                  ImVec2 size = ImVec2(1024, 768)) {
    glm::vec4 trans = mvp * worldPos;
    trans *= 0.5f / trans.w;
    trans += glm::vec4(0.5f, 0.5f, 0, 0);
    trans.y = 1.f - trans.y;
    trans.x *= size.x;
    trans.y *= size.y;
    trans.x += position.x;
    trans.y += position.y;
    return ImVec2(trans.x, trans.y);
}

void knot::DebugPhysics::on_widget_render() {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return;
    }
    auto input = engineOpt->get().get_window_module().lock()->get_input_manager();

    if (input->key_on_trigger(KeyCode::GraveAccent)) {
        active = !active;
        log::debug("TRIGGER");
        log::debug("--");
    }

    if (active) {
        using namespace components;
        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt) {
            return;
        }
        Scene& scene = sceneOpt.value();

        auto engineOpt = Engine::get_active_engine();
        if (!engineOpt) {
            return;
        }
        Engine& m_engine = engineOpt.value();

        entt::registry& registry = scene.get_registry();

        mat4 proj;
        glm::mat4 view;
        auto windowSize = m_engine.get_window_module().lock()->get_window_size();

        auto cameraOpt = EditorCamera::get_active_camera();
        if (!cameraOpt) {
            return;
        }

        auto& editorCamera = cameraOpt.value().get();

        auto goOpt = GameObject::get_game_object_from_component(cameraOpt.value().get());
        if (!goOpt) {
            return;
        }
        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();

        vec3 pos = transform.get_position();
        vec3 lookTarget = editorCamera.get_look_target();
        vec3 up = editorCamera.get_up();

        auto windowWeak = m_engine.get_window_module();
        if (windowWeak.expired()) {
            return;
        }
        auto window = windowWeak.lock();

        float fovY = editorCamera.get_fov();
        float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        float zNear = editorCamera.get_z_near();
        float zFar = editorCamera.get_z_far();

        Hierarchy& hierarchy = go.get_component<Hierarchy>();
        auto parentIDOpt = hierarchy.get_parent();

        if (parentIDOpt) {
            auto sceneOpt = Scene::get_active_scene();
            if (sceneOpt) {
                auto& scene = sceneOpt.value().get();
                auto parentOpt = scene.get_game_object_from_id(parentIDOpt.value());
                if (parentOpt) {
                    auto transform = parentOpt.value().get_component<Transform>();
                    transform.set_rotation_euler({0, 0, 0});

                    auto modelMatrix = transform.get_model_matrix();
                    auto parentDiff = modelMatrix * vec4{0.f, 0.f, 0.f, 1.f};

                    pos += vec3(parentDiff);
                    lookTarget += vec3(parentDiff);
                }
            }
        }

        view = glm::lookAt(pos, lookTarget, up);
        proj = glm::perspective(fovY, aspectRatio, zNear, zFar);

        mat4 projView = view * proj;
        auto physScene = m_engine.get_physics_module().lock()->get_active_Scene();

        const PxRenderBuffer& rendBuffer = physScene.lock().get()->get()->getRenderBuffer();

        ImGui::SetWindowPos("DebugPhysics",ImVec2(0, 0));

        ImGuizmo::BeginFrame();
        ImGui::SetWindowSize("DebugPhysics", ImVec2(m_x, m_y));
        ImGui::Begin("DebugPhysics", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs);
        ImGuizmo::SetDrawlist();
        float windowWidth = m_x;
        float windowHeight = m_y;
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        //    draw_list = ImGui::GetWindowDrawList();
        Transform modelTrans = Transform();
        modelTrans.set_scale(vec3(10, 10, 10));
        mat4 model = modelTrans.get_model_matrix();

        auto entities = registry.view<Transform, Shape, Name>();
        for (auto entity : entities) {
            auto goOpt = scene.get_game_object_from_handle(entity);
            if (!goOpt) {
                continue;
            }

            GameObject go = goOpt.value();
            Transform transform = go.get_component<Transform>();
            Shape& colliderShapeComp = go.get_component<Shape>();
            Name& name = go.get_component<Name>();
            auto colliderShape = colliderShapeComp.get_shape().lock()->get();

            vec3 halfSize = vec3(0, 0, 0);

            if (colliderShape->getGeometryType() == PxGeometryType::eBOX) {
                PxBoxGeometry boxCollider;
                colliderShape->getBoxGeometry(boxCollider);
                halfSize = RigidBody::PxVec3_to_vec3(boxCollider.halfExtents);
            }

            mat4 modelMatrix = mat4(1.0f);
            modelMatrix = translate(mat4(1.0f), transform.get_position() + colliderShapeComp.get_offset_pos()) *
                          toMat4(transform.get_rotation()) * scale(mat4(1.0f), (halfSize * 2.0f));

            ImGuizmo::DrawCubes(value_ptr(view), value_ptr(proj), value_ptr(modelMatrix), 1);
        }

        ImGui::End();
    }
}
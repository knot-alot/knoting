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

    mat4 projView = proj * view;
    auto physScene = m_engine.get_physics_module().lock()->get_active_Scene();

    const PxRenderBuffer& rendBuffer = physScene.lock().get()->get()->getRenderBuffer();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_x, m_y));

    ImGui::Begin("DebugPhysics"
                     , NULL
                     , ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoMove
        );
    draw_list = ImGui::GetWindowDrawList();

    Transform modelTrans = transform

    Transform start = Transform(vec3(0,0,0));
    Transform end = Transform(vec3(0,5,0));

    mat4 startMod = start.get_model_matrix();
    mat4 endMod = end.get_model_matrix();

    mat4 startmvp = projView * startMod;
    mat4 endmvp = projView * endMod;


//    auto entities = registry.view<Transform, Shape>();
//    for(auto entity: entities){
//        auto goOpt = scene.get_game_object_from_handle(entity);
//        if (!goOpt) {
//            continue;
//        }
//
//        GameObject go = goOpt.value();
//        Transform& transform = go.get_component<Transform>();
//        Shape& colliderShapeComp = go.get_component<Shape>();
//        auto colliderShape = colliderShapeComp.get_shape().lock()->get();
//        if(colliderShape->getGeometryType() == PxGeometryType::eBOX){
//            PxBoxGeometry boxCollider;
//            colliderShape->getBoxGeometry(boxCollider);
//            vec3 halfSize = RigidBody::PxVec3_to_vec3(boxCollider.halfExtents);
//            vec3 corners[8] = {
//                vec3(-halfSize.x,-halfSize.y,-halfSize.z),vec3(halfSize.x,-halfSize.y,-halfSize.z),
//                vec3(halfSize.x,-halfSize.y,halfSize.z),vec3(-halfSize.x,-halfSize.y,halfSize.z),
//                vec3(-halfSize.x,halfSize.y,-halfSize.z),vec3(halfSize.x,halfSize.y,-halfSize.z),
//                vec3(halfSize.x,halfSize.y,halfSize.z),vec3(-halfSize.x,halfSize.y,halfSize.z)
//            };
//            ImVec2 screenPos[8];
//            mat4 model = transform.get_model_matrix();
//            for(int i = 0; i < 8; ++i){
//                vec4 clipPos = proj * (view * model  * vec4(corners[i] + transform.get_position(),1.0f));
//                vec2 ndcPos = glm::normalize(vec2(clipPos.x,clipPos.y));
//                screenPos[i] = ImVec2((ndcPos.x + 1.0f) * (windowSize.x/2.0f),(ndcPos.y + 1.0f) * (windowSize.y/2.0f));
//            }
//            ImVec2 startPos = screenPos[0];
//            ImVec2 endPos = screenPos[1];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[1];
//            endPos = screenPos[2];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[2];
//            endPos = screenPos[3];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[3];
//            endPos = screenPos[0];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            //Top Square
//            startPos = screenPos[4];
//            endPos = screenPos[5];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[5];
//            endPos = screenPos[6];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[6];
//            endPos = screenPos[7];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[7];
//            endPos = screenPos[4];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            //Sides
//            startPos = screenPos[0];
//            endPos = screenPos[4];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[1];
//            endPos = screenPos[5];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[2];
//            endPos = screenPos[6];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//            startPos = screenPos[3];
//            endPos = screenPos[7];
//            draw_list->AddLine(startPos,endPos, 0xFFFFFFFF, 2.0f);
//        }
//    }


//    Transform modTrans = Transform();
//    mat4 model = modTrans.get_model_matrix();
//    for(PxU32 i=0; i < rendBuffer.getNbLines(); i++)
//    {
//        const PxDebugLine& line = rendBuffer.getLines()[i];
//        vec3 pos0 = RigidBody::PxVec3_to_vec3(line.pos0);
//        vec3 pos1 = RigidBody::PxVec3_to_vec3(line.pos1);
//
//        //log::debug("pos0: {} pos1: {}",to_string(pos0),to_string(pos1));
//
//        vec4 clip0 = proj * (view * vec4(pos0,0.0f));
//        vec4 clip1 = proj * (view * vec4(pos1,0.0f));
//
//        vec2 startPos = vec2((clip0.x), (clip0.y));
//        startPos = glm::normalize(startPos);
//        vec2 endPos = vec2((clip1.x), (clip1.y));
//        endPos = glm::normalize(endPos);
//
//        startPos.x = (startPos.x + 1) * windowSize.x/2.0f;
//        startPos.y = (startPos.y + 1) * windowSize.y/2.0f;
//
//        endPos.x = (endPos.x + 1) * windowSize.x/2.0f;
//        endPos.y = (endPos.y + 1) * windowSize.y/2.0f;
//        log::debug("pos0: {} pos1: {}",to_string(startPos),to_string(endPos));
//
//
//    }



    ImGui::End();
}
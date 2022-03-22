#include <bgfx/bgfx.h>
#include <knoting/forward_renderer.h>
#include <knoting/instance_mesh.h>
#include <knoting/mesh.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/scene.h>
#include <stb_image.h>
#include <fstream>
#include <string_view>
#include <vector>
#include <knoting/Font.h>

namespace knot {

ForwardRenderer::~ForwardRenderer() {}

ForwardRenderer::ForwardRenderer(Engine& engine) : m_engine(engine) {}

void ForwardRenderer::on_awake() {}

void ForwardRenderer::on_update(double m_delta_time) {
    m_timePassed += (float)m_delta_time;
    m_dt = m_delta_time;
    bgfx::touch(0);

    shadow_pass();
    depth_pass();
    color_pass();
    skybox_pass();
    transparent_pass();
    post_process_pass();
}

void ForwardRenderer::shadow_pass() {}
void ForwardRenderer::depth_pass() {}

void ForwardRenderer::color_pass() {
    using namespace components;
    m_engine.get_framebuffer_manager_module().lock()->clear_all_framebuffers();
    auto fbos = m_engine.get_framebuffer_manager_module().lock();
    auto colorBuffer = fbos->get_framebuffer(FrameBufferType::Color);
    auto idx = colorBuffer.idx;  // TODO check what is render to this buffer via render doc as no editor debugging

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    mat4 invProj;
    glm::mat4 view;
    auto windowSize = m_engine.get_window_module().lock()->get_window_size();
    //=CAMERA===========================
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

        glm::vec3 cameraPos = transform.get_position();
        auto& hierarchy = go.get_component<Hierarchy>();

        vec3 parentPos = vec3(0);

        if (hierarchy.has_parent()) {
            auto parentOpt = scene.get_game_object_from_id(hierarchy.get_parent().value());

            if (parentOpt) {
                auto& parentTransform = parentOpt.value().get_component<Transform>();
                parentPos = parentTransform.get_position();
                vec3 lookTarget = editorCamera.get_look_target();
                editorCamera.set_look_target(lookTarget);

                vec3 diffCameraPosLookTager = lookTarget - cameraPos;

                cameraPos = parentTransform.get_model_matrix() * vec4(cameraPos, 1.0f);

                diffCameraPosLookTager = toMat4(parentTransform.get_rotation()) * vec4(diffCameraPosLookTager, 1.0f);
                editorCamera.set_look_target(cameraPos + diffCameraPosLookTager);
            }
        }

        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            view = glm::lookAt(cameraPos, lookTarget, up);
            glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);
            invProj = inverse(proj);
            bgfx::setViewTransform(idx, &view[0][0], &proj[0][0]);
        }

        //=PARTICLES SYSTEM=======================
        auto particles = registry.view<Particles>();



        for (auto& e : particles) {
            auto gooOpt = scene.get_game_object_from_handle(e);
            if (!gooOpt) {
                continue;
            }

            GameObject goo = gooOpt.value();
            Particles& ps = goo.get_component<Particles>();
            auto& hierarchy = goo.get_component<Hierarchy>();

            vec3 parentPos = vec3(0);

            if (hierarchy.has_parent()) {
                auto parentOpt = scene.get_game_object_from_id(hierarchy.get_parent().value());

                if (parentOpt) {
                    auto& parentTransform = parentOpt.value().get_component<Transform>();
                    parentPos = parentTransform.get_position();
                    vec3 lookTarget = editorCamera.get_look_target();
                    editorCamera.set_look_target(lookTarget);

                    vec3 diffCameraPosLookTager = lookTarget - cameraPos;

                    cameraPos = parentTransform.get_model_matrix() * vec4(cameraPos, 1.0f);

                    diffCameraPosLookTager =
                        toMat4(parentTransform.get_rotation()) * vec4(diffCameraPosLookTager, 1.0f);
                    editorCamera.set_look_target(cameraPos + diffCameraPosLookTager);
                }
            }
            const bx::Vec3 eye =
                bx::Vec3(transform.get_position().x, transform.get_position().y, transform.get_position().z);
            float viewMtx[16];

            bx::mtxLookAt(viewMtx, bx::load<bx::Vec3>(&eye.x), bx::load<bx::Vec3>(&lookTarget.x),
                          bx::load<bx::Vec3>(&up.x));

            ps.update(m_dt * 0.1);
            ps.render(idx, viewMtx, eye);
        }

    }

    //=SPOT LIGHTS======================
    // TODO consider writing a system to skip this system if light data has not changed between frames
    // TODO consider writing 2 systems 1 for static lights where this data is set 'on_awake'
    auto lights = registry.view<Transform, SpotLight>();
    m_lightData.set_spotlight_count(lights.size_hint());
    m_lightData.clear_spotlight();
    for (auto& e : lights) {
        auto goOpt = scene.get_game_object_from_handle(e);
        if (!goOpt) {
            continue;
        }
        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();
        SpotLight& spotLight = go.get_component<SpotLight>();

        // Packed uniform data
        m_lightData.push_spotlight_pos_outer_rad(vec4(transform.get_position(), spotLight.get_outer_radius()));
        m_lightData.push_spotlight_color_inner_rad(vec4(spotLight.get_color(), spotLight.get_inner_radius()));
    }

    //=SKYBOX=================================

    auto skyboxes = registry.view<InstanceMesh, SkyBox>();
    for (auto& e : skyboxes) {
        auto goOpt = scene.get_game_object_from_handle(e);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        InstanceMesh& mesh = go.get_component<InstanceMesh>();
        SkyBox& skyBox = go.get_component<SkyBox>();

        bgfx::setVertexBuffer(idx, mesh.get_vertex_buffer());

        if (isValid(mesh.get_index_buffer())) {
            bgfx::setIndexBuffer(mesh.get_index_buffer());
        }

        skyBox.set_uniforms();

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_DEPTH_TEST_LEQUAL);
        bgfx::submit(idx, skyBox.get_program());
    }

    //=PBR PIPELINE===========================

    auto entities = registry.view<Transform, InstanceMesh, Material, Name>();
    for (auto& e : entities) {
        auto goOpt = scene.get_game_object_from_handle(e);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();
        InstanceMesh& mesh = go.get_component<InstanceMesh>();
        Material& material = go.get_component<Material>();
        Name& name = go.get_component<Name>();

        bgfx::setTransform(value_ptr(transform.get_model_matrix()));

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(idx, mesh.get_vertex_buffer());

        if (isValid(mesh.get_index_buffer())) {
            bgfx::setIndexBuffer(mesh.get_index_buffer());
        }

        // Bind spotlight uniforms
        m_lightData.set_spotlight_uniforms();
        // Bind Uniforms & textures.
        material.set_uniforms();

        bgfx::setState(0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                       BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(idx, material.get_program());
    }
    // font
    auto f = registry.view<Font>();
    for (auto& e : f) {
        auto goOpt = scene.get_game_object_from_handle(e);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Font& font = go.get_component<Font>();
        font.get_textBuffer()->submitTextBuffer(font.get_text(), 1);
    }
}

void ForwardRenderer::skybox_pass() {}
void ForwardRenderer::transparent_pass() {}
void ForwardRenderer::post_process_pass() {}

void ForwardRenderer::on_late_update() {}
void ForwardRenderer::on_destroy() {}

}  // namespace knot

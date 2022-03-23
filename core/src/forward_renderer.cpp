#include <bgfx/bgfx.h>
#include <knoting/forward_renderer.h>
#include <knoting/instance_mesh.h>
#include <knoting/mesh.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/post_processing.h>
#include <knoting/scene.h>
#include <stb_image.h>
#include <fstream>
#include <string_view>
#include <vector>

#include <bx/math.h>

namespace knot {

ForwardRenderer::~ForwardRenderer() {}

ForwardRenderer::ForwardRenderer(Engine& engine) : m_engine(engine) {}

void ForwardRenderer::on_awake() {}

void ForwardRenderer::on_update(double m_delta_time) {
    m_timePassed += (float)m_delta_time;
    m_dt = m_delta_time;
    m_engine.get_framebuffer_manager_module().lock()->clear_all_framebuffers();
    auto framebufferManager = m_engine.get_framebuffer_manager_module().lock();

    auto depth = framebufferManager->get_framebuffer(FrameBufferType::Depth);
    auto color = framebufferManager->get_framebuffer(FrameBufferType::Color);
    auto gui = framebufferManager->get_framebuffer(FrameBufferType::Gui);

    bgfx::setViewFrameBuffer(depth.idx, depth);
    depth_pass(depth.idx);

    bgfx::setViewFrameBuffer(color.idx, color);
    color_pass(color.idx);

    bgfx::setViewFrameBuffer(gui.idx, gui);
    gui_pass(gui.idx);

    // RENDER TO BACK BUFFER
    bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
    post_process_pass(0);
}

void ForwardRenderer::shadow_pass(uint16_t idx) {}
void ForwardRenderer::depth_pass(uint16_t idx) {
    using namespace components;
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

        const glm::vec3 pos = transform.get_position();
        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            view = glm::lookAt(pos, lookTarget, up);
            glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);
            bgfx::setViewTransform(idx, &view[0][0], &proj[0][0]);
        }
    }

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

        // Bind Uniforms & textures.
        material.set_uniforms();

        bgfx::setState(0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(idx, material.get_program());
    }
}

void ForwardRenderer::color_pass(uint16_t idx) {
    using namespace components;

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

        const glm::vec3 pos = transform.get_position();
        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            view = glm::lookAt(pos, lookTarget, up);
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
}

void ForwardRenderer::gui_pass(uint16_t idx) {
    using namespace components;
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

        const glm::vec3 pos = transform.get_position();
        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            view = glm::lookAt(pos, lookTarget, up);
            glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);
            bgfx::setViewTransform(idx, &view[0][0], &proj[0][0]);
        }
    }

    auto entities = registry.view<Transform, InstanceMesh, Material, Name, PostProcessing>();
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

        // Bind Uniforms & textures.
        material.set_uniforms();

        bgfx::setState(0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                       BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(idx, material.get_program());
    }
}

void ForwardRenderer::transparent_pass(uint16_t idx) {}

void ForwardRenderer::post_process_pass(uint16_t idx) {
    using namespace components;
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    //=SET BGFX GLOBAL UNIFORMS===============

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

        const glm::vec3 pos = transform.get_position();
        const glm::vec3 lookTarget = editorCamera.get_look_target();
        const glm::vec3 up = editorCamera.get_up();

        const float fovY = editorCamera.get_fov();
        const float aspectRatio = float((float)windowSize.x / (float)windowSize.y);
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            view = glm::lookAt(pos, lookTarget, up);
            glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);
            bgfx::setViewTransform(idx, &view[0][0], &proj[0][0]);
        }
    }

    //=POST PROCESSING========================
    {
        auto postProcessEntities = registry.view<Transform, PostProcessing, Name, InstanceMesh>();
        for (auto& e : postProcessEntities) {
            auto goOpt = scene.get_game_object_from_handle(e);
            if (!goOpt) {
                continue;
            }

            GameObject go = goOpt.value();
            Transform& transform = go.get_component<Transform>();
            InstanceMesh& mesh = go.get_component<InstanceMesh>();
            Name& name = go.get_component<Name>();
            PostProcessing& postProcessing = go.get_component<PostProcessing>();

            bgfx::setTransform(value_ptr(transform.get_model_matrix()));
            bgfx::setVertexBuffer(idx, mesh.get_vertex_buffer());

            if (isValid(mesh.get_index_buffer())) {
                bgfx::setIndexBuffer(mesh.get_index_buffer());
            }

            auto frameBuffers = m_engine.get_framebuffer_manager_module().lock();

            auto colTex = frameBuffers->get_texture_attachments(FrameBufferType::Color);
            auto guiTex = frameBuffers->get_texture_attachments(FrameBufferType::Gui);

            postProcessing.set_color_render_texture(colTex[0]);
            postProcessing.set_gui_render_texture(guiTex[0]);

            postProcessing.set_uniforms();

            bgfx::setState(0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                           BGFX_STATE_DEPTH_TEST_LESS);

            bgfx::submit(idx, postProcessing.get_program());
        }
    }
}

void ForwardRenderer::on_late_update() {}
void ForwardRenderer::on_destroy() {}

}  // namespace knot

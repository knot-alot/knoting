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

namespace knot {

ForwardRenderer::~ForwardRenderer() {}

ForwardRenderer::ForwardRenderer(Engine& engine) : m_engine(engine) {}

void ForwardRenderer::on_render() {
    using namespace components;
    clear_framebuffer();
    bgfx::touch(0);

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

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
        const float aspectRatio = float((float)get_window_width() / (float)get_window_height());
        const float zNear = editorCamera.get_z_near();
        const float zFar = editorCamera.get_z_far();

        // Set view and projection matrix for view 0.
        {
            glm::mat4 view;
            view = glm::lookAt(pos, lookTarget, up);
            glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);

            bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
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
        bgfx::setVertexBuffer(0, mesh.get_vertex_buffer());

        if (isValid(mesh.get_index_buffer())) {
            bgfx::setIndexBuffer(mesh.get_index_buffer());
        }

        // Bind spotlight uniforms
        m_lightData.set_spotlight_uniforms();
        // Bind Uniforms & textures.
        material.set_uniforms();

        // TODO enable MSAA in bgfx
        bgfx::setState(0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                       BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(0, material.get_program());
    }

    auto skyboxes = registry.view<Transform, InstanceMesh, SkyBox, Name>();
    for (auto& e : skyboxes) {
        auto goOpt = scene.get_game_object_from_handle(e);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        Transform& transform = go.get_component<Transform>();
        InstanceMesh& mesh = go.get_component<InstanceMesh>();
        SkyBox& skyBox = go.get_component<SkyBox>();
        Name& name = go.get_component<Name>();

        bgfx::setTransform(value_ptr(transform.get_model_matrix()));
        bgfx::setVertexBuffer(0, mesh.get_vertex_buffer());

        if (isValid(mesh.get_index_buffer())) {
            bgfx::setIndexBuffer(mesh.get_index_buffer());
        }

        m_lightData.set_spotlight_uniforms();
        skyBox.set_uniforms();

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
        bgfx::submit(0, skyBox.get_program());
    }

    bgfx::frame();
}

void ForwardRenderer::on_post_render() {}

void ForwardRenderer::on_awake() {}

void ForwardRenderer::on_update(double m_delta_time) {
    m_timePassed += (float)m_delta_time;
}

void ForwardRenderer::on_late_update() {}

void ForwardRenderer::on_destroy() {}

void ForwardRenderer::recreate_framebuffer(uint16_t width, uint16_t height, uint16_t id) {
    bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
    bgfx::setViewClear(id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL, (uint32_t)m_clearColor);
    bgfx::setViewRect(id, 0, 0, width, height);
}

void ForwardRenderer::clear_framebuffer(uint16_t id) {
    bgfx::setViewClear(id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, (uint32_t)m_clearColor);
}

int ForwardRenderer::get_window_width() {
    return m_engine.get_window_module().lock()->get_window_width();
}

int ForwardRenderer::get_window_height() {
    return m_engine.get_window_module().lock()->get_window_height();
}

}  // namespace knot

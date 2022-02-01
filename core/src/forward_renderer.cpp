#include <bgfx/bgfx.h>
#include <knoting/forward_renderer.h>
#include <knoting/mesh.h>
#include <knoting/texture.h>

#include <knoting/engine.h>
#include <knoting/stb_image.h>
#include <fstream>
#include <string_view>

namespace knot {

ForwardRenderer::~ForwardRenderer() {}

bgfx::TextureHandle load_texture_2d(const std::string& path,
                                    bool usingMipMaps = true,
                                    bool usingAnisotropicFiltering = true) {
    constexpr char L_PATH_TEXTURE[] = "../res/textures/";
    std::string fullPath = L_PATH_TEXTURE + path;
    std::filesystem::path fs_path = fullPath;

    if (exists(fs_path)) {
        log::debug(fullPath + " - Exists");
    } else {
        log::error(fullPath + " - does not Exist");
        return BGFX_INVALID_HANDLE;
    }

    // load image with stb_image
    glm::ivec2 img_size;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(fullPath.c_str(), &img_size.x, &img_size.y, &channels, 0);
    int numberOfLayers = 1;

    uint32_t textureFlags{0};
    // TODO check if bgfx has anisotropic is supported flag

    if (usingAnisotropicFiltering) {
        textureFlags =
            BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
    } else {
        textureFlags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
    }

    // TODO stbi does not support mips find a way to get mips working
    usingMipMaps = false;
    if (usingMipMaps) {
        textureFlags += BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    }

    bgfx::TextureHandle textureHandle =
        bgfx::createTexture2D(img_size.x, img_size.y, usingMipMaps, numberOfLayers, bgfx::TextureFormat::RGBA8,
                              textureFlags, bgfx::copy(data, img_size.x * img_size.y * channels));

    if (!bgfx::isValid(textureHandle)) {
        log::error("Error loading texture " + path);
        return BGFX_INVALID_HANDLE;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    return textureHandle;
}

ForwardRenderer::ForwardRenderer(Engine& engine) : m_engine(engine) {
//    m_shaderProgram.load_shader("cubes","vs_cubes.bin", "fs_cubes.bin");
    m_shaderProgram.load_shader("bump","vs_bump.bin", "fs_bump.bin");
    m_cube.create_cube();

    m_instancingSupported = false;


    m_numLights = 4;
    u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, m_numLights);
    u_lightRgbInnerR = bgfx::createUniform("u_lightRgbInnerR", bgfx::UniformType::Vec4, m_numLights);

    //        // Create program from shaders.
    //        m_program = loadProgram(m_instancingSupported ? "vs_bump_instanced" : "vs_bump", "fs_bump");

    // TODO Write Texture class

    // Create texture sampler uniforms & load textures
    s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    m_textureColor = load_texture_2d("UV_Grid_test.png");

    s_texNormal = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
    m_textureNormal = load_texture_2d("normal_tiles_1k.png");



}

void ForwardRenderer::on_render() {
    clear_framebuffer();

    bgfx::touch(0);

    const glm::vec3 at = {0.0f, 0.0f, 0.0f};
    const glm::vec3 eye = {0.0f, 0.0f, -7.0f};
    const glm::vec3 up = {0.0f, 1.0f, 0.0f};

    const float fovY = glm::radians(60.0f);
    const float aspectRatio = float((float)get_window_width() / (float)get_window_height());
    const float zNear = 0.1f;
    const float zFar = 100.0f;

    log::debug(m_timePassed);

    // Set view and projection matrix for view 0.
    {
        glm::mat4 view;
        view = glm::lookAt(eye, at, up);
        glm::mat4 proj = glm::perspective(fovY, aspectRatio, zNear, zFar);

        bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
    }

    float lightPosRadius[4][4];
    for (uint32_t ii = 0; ii < m_numLights; ++ii) {
        lightPosRadius[ii][0] = glm::sin( (m_timePassed *(0.1f + ii*0.17f) + ii*glm::half_pi<float>()*1.37f ) )*15.0f;
        lightPosRadius[ii][1] = glm::cos( (m_timePassed *(0.2f + ii*0.29f) + ii*glm::half_pi<float>()*1.49f ) )*15.0f;
        lightPosRadius[ii][2] = 15.5f;
        lightPosRadius[ii][3] = 17.0f;
    }

    bgfx::setUniform(u_lightPosRadius, lightPosRadius, m_numLights);

    float lightRgbInnerR[4][4] = {
        {1.0f, 0.7f, 0.2f, 0.8f},
        {0.7f, 0.2f, 1.0f, 0.8f},
        {0.2f, 1.0f, 0.7f, 0.8f},
        {1.0f, 0.4f, 0.2f, 0.8f},
    };

    bgfx::setUniform(u_lightRgbInnerR, lightRgbInnerR, m_numLights);

    for (uint32_t yy = 0; yy < 11; ++yy) {
        for (uint32_t xx = 0; xx < 11; ++xx) {
            glm::mat4 mtx = glm::identity<glm::mat4>();
            mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 30.0f));
            mtx *= glm::yawPitchRoll(m_timePassed + xx * 0.21f, m_timePassed + yy * 0.37f, 0.0f);
            bgfx::setTransform(&mtx[0][0]);

            // Set vertex and index buffer.
            bgfx::setVertexBuffer(0, m_cube.get_vertex_buffer());
            bgfx::setIndexBuffer(m_cube.get_index_buffer());

            // Bind textures.
            bgfx::setTexture(0, s_texColor,  m_textureColor);
            bgfx::setTexture(1, s_texNormal, m_textureNormal);

            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                           BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA);

            bgfx::submit(0, m_shaderProgram.get_program());
        }
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

#include <bgfx/bgfx.h>
#include <knoting/forward_renderer.h>
#include <knoting/mesh.h>
#include <knoting/texture.h>

#include <knoting/engine.h>
#include <fstream>
#include <string_view>

namespace knot {

ForwardRenderer::~ForwardRenderer() {}

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
    static void init() {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
    static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},   {-1.0f, -1.0f, 1.0f, 0xff00ff00},
    {1.0f, -1.0f, 1.0f, 0xff00ffff},   {-1.0f, 1.0f, -1.0f, 0xffff0000}, {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};
static const uint16_t s_cubeTriList[] = {2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4,
                                         3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6};

const bgfx::Memory* loadMemory(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    const bgfx::Memory* mem = bgfx::alloc(uint32_t(size + 1));
    if (file.read((char*)mem->data, size)) {
        mem->data[mem->size - 1] = '\0';
        return mem;
    }
    return nullptr;
}

bgfx::ProgramHandle loadProgram(const char* vsName, const char* fsName) {
    bgfx::ShaderHandle vs = bgfx::createShader(loadMemory(vsName));
    bgfx::ShaderHandle fs = bgfx::createShader(loadMemory(fsName));
    return bgfx::createProgram(vs, fs, true);
}

ForwardRenderer::ForwardRenderer(Engine& engine) : m_engine(engine) {
    PosColorVertex::init();

    mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
    mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    m_timePassed = 0.0f;
}

void ForwardRenderer::on_render() {
    clear_framebuffer();

    bgfx::touch(0);

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 0.0f, -35.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(get_window_width()) / get_window_height(), 0.1f, 100.0f);
    bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
    bgfx::setViewRect(0, 0, 0, uint16_t(get_window_width()), uint16_t(get_window_height()));
    for (uint32_t yy = 0; yy < 11; ++yy) {
        for (uint32_t xx = 0; xx < 11; ++xx) {
            glm::mat4 mtx = glm::identity<glm::mat4>();
            mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 0.0f));
            mtx *= glm::yawPitchRoll(m_timePassed + xx * 0.21f, m_timePassed + yy * 0.37f, 0.0f);
            bgfx::setTransform(&mtx[0][0]);
            bgfx::setVertexBuffer(0, mVbh);
            bgfx::setIndexBuffer(mIbh);
            bgfx::setState(BGFX_STATE_DEFAULT);
            bgfx::submit(0, shaderProgram.get_program());
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

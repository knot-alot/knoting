
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <filesystem>

namespace knot {
bgfx::VertexLayout VertexLayout::meshVertexLayout;
bgfx::VertexLayout CubeVertexLayout::ms_layout;

}  // namespace knot

namespace knot {

Mesh::Mesh() {}

Mesh::~Mesh() {}

void knot::Mesh::load_mesh(const std::string& localTexturePath) {
    std::string fullPath = PATH_MODELS + localTexturePath;
    std::filesystem::path fs_path = std::filesystem::path(fullPath);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    uint16_t size;
    char* raw_image;

    if (exists(fs_path)) {
        log::debug("file loaded at : " + fullPath);

    } else {
        log::error("file : " + fullPath + " does not exist");
    }
}

void Mesh::create_cube() {
    std::vector<uint16_t> tempIndex = {
        2, 1, 0,
        2, 3, 1,
        5, 6, 4,
        7, 6, 5,
        4, 2, 0,
        6, 2, 4,
        3, 5, 1,
        3, 7, 5,
        1, 4, 0,
        1, 5, 4,
        6, 3, 2,
        7, 3, 6};

    m_indexBuffer = std::make_shared<IndexBuffer>();
    m_indexBuffer->set_index_buffer(tempIndex);

    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(m_indexBuffer->get_index_start(), m_indexBuffer->get_memory_size()));

    CubeVertexLayout::init();

    m_cubeVertices = {
        {-1.0f, 1.0f , 1.0f , 0xff000000},
        {1.0f , 1.0f , 1.0f , 0xff0000ff},
        {-1.0f, -1.0f, 1.0f , 0xff00ff00},
        {1.0f , -1.0f, 1.0f , 0xff00ffff},
        {-1.0f, 1.0f , -1.0f, 0xffff0000},
        {1.0f , 1.0f , -1.0f, 0xffff00ff},
        {-1.0f, -1.0f, -1.0f, 0xffffff00},
        {1.0f , -1.0f, -1.0f, 0xffffffff},
    };

    m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(&m_cubeVertices[0], sizeof(m_cubeVertices[0]) * m_cubeVertices.size()), CubeVertexLayout::ms_layout);
}

}  // namespace knot

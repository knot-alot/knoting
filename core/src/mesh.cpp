#include <knoting/log.h>
#include <knoting/mesh.h>
#include <filesystem>

namespace knot {
namespace components {

Mesh::Mesh() : Asset{AssetType::MESH, "", "fallbackMesh"} {}
Mesh::Mesh(const std::string& path) : Asset{AssetType::MESH, path, "fallbackMesh"} {}

Mesh::~Mesh() {}

void Mesh::on_awake() {}

void Mesh::on_destroy() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
}

void Mesh::load_mesh(const std::string& localTexturePath) {
    std::string fullPath = PATH_MODELS + localTexturePath;
    std::filesystem::path fs_path = std::filesystem::path(fullPath);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    if (std::filesystem::exists(fs_path)) {
        log::debug("file loaded at : " + fullPath);

    } else {
        log::error("file : " + fullPath + " does not exist");
    }
}

void Mesh::create_cube() {
    std::vector<uint16_t> tempIndex = {
        0,  2,  1,  1,  2,  3,  4,  5,  6,  5,  7,  6,

        8,  10, 9,  9,  10, 11, 12, 13, 14, 13, 15, 14,

        16, 18, 17, 17, 18, 19, 20, 21, 22, 21, 23, 22,
    };

    m_indexBuffer = std::make_shared<IndexBuffer>();
    m_indexBuffer->set_index_buffer(tempIndex);

    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(&m_indexBuffer->get_index_start(), m_indexBuffer->get_memory_size()));

    VertexLayout::init();
    m_vertexLayout = {
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0x7fff, 0},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0, 0x7fff},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0x7fff, 0x7fff},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0, 0},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0x7fff, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0, 0x7fff},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0x7fff, 0x7fff},
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0x7fff, 0},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0, 0x7fff},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0x7fff, 0x7fff},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0, 0},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0x7fff, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0, 0x7fff},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0x7fff, 0x7fff},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0x7fff, 0},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0, 0x7fff},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0x7fff, 0x7fff},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0, 0},
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0x7fff, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0, 0x7fff},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0x7fff, 0x7fff},
    };

    m_vbh =
        bgfx::createVertexBuffer(bgfx::makeRef(&m_vertexLayout[0], sizeof(m_vertexLayout[0]) * m_vertexLayout.size()),
                                 VertexLayout::s_meshVertexLayout);
}

void Mesh::generate_default_asset() {}

}  // namespace components
}  // namespace knot

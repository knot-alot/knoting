#pragma once

#include <bgfx/bgfx.h>
#include <bx/pixelformat.h>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>

// TODO Add const variables headers
constexpr char PATH_SHADER[] = "../res/shaders/";
constexpr char PATH_TEXTURE[] = "../res/textures/";
constexpr char PATH_MODELS[] = "../res/models/";
//end TODO

namespace knot {
class IndexBuffer;
class VertexLayout;
}  // namespace knot

namespace knot {

class Mesh {
   public:
    Mesh();
    ~Mesh();

    void load_mesh(const std::string& localTexturePath);
    void create_cube();

    bgfx::VertexBufferHandle get_vertex_buffer() { return m_vbh; }
    bgfx::IndexBufferHandle get_index_buffer() { return m_ibh; }

   private:
    std::vector<VertexLayout> m_vertexLayout;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

   private:
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
};

class IndexBuffer {
   public:
    IndexBuffer(){};
    void set_index_buffer(std::vector<uint16_t>& in_indices) { indices = in_indices; }
    size_t get_memory_size() { return sizeof(indices[0]) * indices.size(); }
    uint16_t* get_index_start() { return &indices[0]; }

   private:
    std::vector<uint16_t> indices;
};

inline uint32_t encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f) {
    const float src[] = {
        _x * 0.5f + 0.5f,
        _y * 0.5f + 0.5f,
        _z * 0.5f + 0.5f,
        _w * 0.5f + 0.5f,
    };
    uint32_t dst;
    bx::packRgba8(&dst, src);
    return dst;
}

class VertexLayout {
    // TODO update clean this to GLM variables
   public:
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_normal;
    uint32_t m_tangent;
    int16_t m_u;
    int16_t m_v;

    static void init() {
        meshVertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
            .end();

        // TODO when animations are impl
        //.add(bgfx::Attrib::Count, 1, bgfx::AttribType::Uint8)
        //.add(bgfx::Attrib::Weight, 1, bgfx::AttribType::Float)
        //.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        // end TODO
    }

    static bgfx::VertexLayout meshVertexLayout;
};

}  // namespace knot

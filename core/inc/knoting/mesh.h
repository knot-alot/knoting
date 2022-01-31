#pragma once

#include <bgfx/bgfx.h>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>

constexpr char PATH_SHADER[] = "../res/shaders/";
constexpr char PATH_TEXTURE[] = "../res/textures/";
constexpr char PATH_MODELS[] = "../res/models/";

namespace knot {
class IndexBuffer;
class VertexLayout;
class CubeVertexLayout;

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
    std::vector<CubeVertexLayout> m_cubeVertices;

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

class VertexLayout {
   public:
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
    glm::vec2 m_uv;

    static void init() {
        meshVertexLayout.begin()
            .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal,    4, bgfx::AttribType::Float, true, true)
            .add(bgfx::Attrib::Tangent,   4, bgfx::AttribType::Float, true, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
            .end();

            // TODO when animations are impl
            //.add(bgfx::Attrib::Count, 1, bgfx::AttribType::Uint8)
            //.add(bgfx::Attrib::Weight, 1, bgfx::AttribType::Float)
            //.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            // end TODO
    }
    static bgfx::VertexLayout meshVertexLayout;
};

struct CubeVertexLayout {
    glm::vec3 position;
    uint32_t abgr;
    static void init() {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

}  // namespace knot

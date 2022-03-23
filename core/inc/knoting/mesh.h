#pragma once

#include <bgfx/bgfx.h>
#include <bx/pixelformat.h>
#include <knoting/asset.h>
#include <knoting/component.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <string>
#include <vector>

// TODO Add const variables headers

// end TODO

namespace knot {
namespace components {
class IndexBuffer;
class VertexLayout;
}  // namespace components
}  // namespace knot

namespace knot {
namespace components {

class Mesh : public Asset, public Component<Mesh> {
   public:
    Mesh();
    Mesh(const std::string& path);
    ~Mesh();

    void on_awake() override;
    void on_destroy() override;

    void generate_default_asset() override;
    void generate_post_process_plane();

    void create_cube();

    bgfx::VertexBufferHandle get_vertex_buffer() { return m_vbh; }
    bgfx::IndexBufferHandle get_index_buffer() { return m_ibh; }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_assetType), CEREAL_NVP(m_fallbackName), CEREAL_NVP(m_fullPath), CEREAL_NVP(m_assetName),
                CEREAL_NVP(m_vertexLayout), CEREAL_NVP(m_indexBuffer), CEREAL_NVP(m_splitResult));
    }

   private:
    bool internal_load_obj(const std::string& path);
    std::vector<std::string> split(std::string s, const std::string& t);

   private:
    std::vector<VertexLayout> m_vertexLayout;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::vector<std::string> m_splitResult;

   private:
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
};

class IndexBuffer {
   public:
    void set_index_buffer(const std::vector<unsigned int>& in_indices) { m_indices = in_indices; }
    size_t get_memory_size() { return sizeof(m_indices[0]) * m_indices.size(); }
    unsigned int& get_index_start() { return m_indices[0]; }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::make_nvp("indices", m_indices));
    }

   private:
    std::vector<unsigned int> m_indices;
};

inline uint32_t encode_normal_rgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f) {
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
    float m_u;
    float m_v;

    static void init() {
        s_meshVertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
            .end();

        // TODO when animations are impl
        //.add(bgfx::Attrib::Count, 1, bgfx::AttribType::Uint8)
        //.add(bgfx::Attrib::Weight, 1, bgfx::AttribType::Float)
        //.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        // end TODO
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::make_nvp("x", m_x), cereal::make_nvp("y", m_y), cereal::make_nvp("z", m_z),
                cereal::make_nvp("normal", m_normal), cereal::make_nvp("tangent", m_tangent),
                cereal::make_nvp("u", m_u), cereal::make_nvp("v", m_u));
    }

    inline static bgfx::VertexLayout s_meshVertexLayout;
};
}  // namespace components
}  // namespace knot

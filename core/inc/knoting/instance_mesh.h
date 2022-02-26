#pragma once
#include <knoting/asset.h>
#include <knoting/asset_manager.h>
#include <knoting/mesh.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <string>
#include <vector>

namespace knot {
namespace components {
class InstanceMesh {
   public:
    InstanceMesh();
    InstanceMesh(const std::string& path);

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    bgfx::VertexBufferHandle get_vertex_buffer() { return m_mesh->get_vertex_buffer(); }
    bgfx::IndexBufferHandle get_index_buffer() { return m_mesh->get_index_buffer(); }

    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_path));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_path));
        m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
    }

   private:
    std::shared_ptr<components::Mesh> m_mesh;
    std::string m_path;
};
}  // namespace components

}  // namespace knot

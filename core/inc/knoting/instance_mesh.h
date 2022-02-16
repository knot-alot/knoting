#pragma once
#include <knoting/asset.h>
#include <knoting/asset_manager.h>
#include <knoting/mesh.h>
#include <knoting/types.h>
#include <string>
#include <vector>

namespace knot {

class InstanceMesh {
   public:
    InstanceMesh();
    InstanceMesh(const std::string& path);

    bgfx::VertexBufferHandle get_vertex_buffer() { return m_mesh->get_vertex_buffer(); }
    bgfx::IndexBufferHandle get_index_buffer() { return m_mesh->get_index_buffer(); }

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================
   private:
    std::shared_ptr<components::Mesh> m_mesh;
    std::string m_path;
};
}  // namespace knot

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

    bgfx::VertexBufferHandle* get_vertex_buffer() { return m_instance_mesh->get_vertex_buffer(); }
    bgfx::IndexBufferHandle* get_index_buffer() { return m_instance_mesh->get_index_buffer(); }

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================
    components::Mesh* get_mesh(){return m_instance_mesh.get();};
   private:
    std::shared_ptr<components::Mesh> m_instance_mesh;
    std::string m_path;
};
}  // namespace knot

#include <knoting/instance_mesh.h>

namespace knot {
InstanceMesh::InstanceMesh() {}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    log::info("LOOKING FOR MESH FOR INSTANCED LAD");
    m_instance_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
}

void InstanceMesh::on_destroy() {}

}  // namespace knot

#include <knoting/instance_mesh.h>

namespace knot {
InstanceMesh::InstanceMesh() {}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
}

void InstanceMesh::on_destroy() {}

}  // namespace knot

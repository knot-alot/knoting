#include <knoting/instance_mesh.h>

namespace knot {
namespace components {
InstanceMesh::InstanceMesh() {}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
    for (int i = 0; i < 100; ++i) {
        m_paintData.emplace_back(vec4(0));
    }
}


void InstanceMesh::on_destroy() {}

void InstanceMesh::addContactPoint(vec3 position, Team team) {
    m_paintData.pop_front();
    m_paintData.emplace_back(vec4(position, static_cast<size_t>(team)));
}

}  // namespace components
}  // namespace knot

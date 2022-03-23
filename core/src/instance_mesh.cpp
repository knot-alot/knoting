#include <knoting/instance_mesh.h>

namespace knot {
namespace components {
InstanceMesh::InstanceMesh() {
    m_start = 0;
    m_end = 99;
    m_size = 100;
}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
    m_paintData.fill(vec4(0));
}

void InstanceMesh::on_destroy() {}

void InstanceMesh::addContactPoint(vec3 position, Team team) {
        m_paintData[m_start] = vec4(position, static_cast<size_t>(team));
        int tempStart = m_start;
        m_start = (m_start + 1) % Mesh::NUM_RANDOM_POINTS;
        m_end =  tempStart;
}

}  // namespace components
}  // namespace knot

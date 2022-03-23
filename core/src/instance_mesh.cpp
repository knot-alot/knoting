#include <knoting/instance_mesh.h>

namespace knot {
namespace components {
InstanceMesh::InstanceMesh() {}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
    auto& randomPoints = m_mesh->get_random_points();
    bool isRed = false;
    for(int i = 0; i < m_paintData.max_size(); ++i){
        m_paintData[i] = vec4(vec3(0,0,1),isRed + 1);
        log::debug("{}", to_string(m_paintData[i]));
        isRed = !isRed;
    }
}

void InstanceMesh::on_destroy() {}

}  // namespace components
}  // namespace knot

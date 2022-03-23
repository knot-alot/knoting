#include <knoting/instance_mesh.h>

namespace knot {
namespace components {
InstanceMesh::InstanceMesh() {
}
InstanceMesh::InstanceMesh(const std::string& path) : m_path(path) {}

void InstanceMesh::on_awake() {
    m_mesh = AssetManager::load_asset<components::Mesh>(m_path).lock();
    int i = 0;
    for(int x = -5; x < 5; ++x){
        for(int z = -5; z < 5;++z){
            m_paintData.emplace_back(vec4(x,0,z,1));
            i++;
        }
    }
    //m_paintData.fill(vec4(rand()%10,0,rand()% 10,1));
}

void InstanceMesh::on_destroy() {}

void InstanceMesh::addContactPoint(vec3 position, Team team) {
    m_paintData.pop_front();
    m_paintData.emplace_back(vec4(position, static_cast<size_t>(team)));

}

}  // namespace components
}  // namespace knot

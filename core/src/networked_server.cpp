#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine) {}
void NetworkedServer::on_awake() {
    Subsystem::on_awake();
}
void NetworkedServer::on_update(double m_delta_time) {
    Subsystem::on_update(m_delta_time);
}
void NetworkedServer::on_fixed_update() {
    Subsystem::on_fixed_update();
}
void NetworkedServer::on_late_update() {
    Subsystem::on_late_update();
}
void NetworkedServer::on_destroy() {
    Subsystem::on_destroy();
}
}  // namespace knot
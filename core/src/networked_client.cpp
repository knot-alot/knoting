#include <knoting/networked_client.h>

namespace knot {
NetworkedClient::~NetworkedClient() {}

NetworkedClient::NetworkedClient(Engine& engine) : m_engine(engine) {}
void NetworkedClient::on_awake() {
    Subsystem::on_awake();
}
void NetworkedClient::on_update(double m_delta_time) {
    Subsystem::on_update(m_delta_time);
}
void NetworkedClient::on_fixed_update() {
    Subsystem::on_fixed_update();
}
void NetworkedClient::on_late_update() {
    Subsystem::on_late_update();
}
void NetworkedClient::on_destroy() {
    Subsystem::on_destroy();
}
}  // namespace knot
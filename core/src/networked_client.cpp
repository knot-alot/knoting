#include <knoting/networked_client.h>

namespace knot {
using namespace yojimbo;
NetworkedClient::~NetworkedClient() {}

NetworkedClient::NetworkedClient(Engine& engine) : m_engine(engine) {
    InitializeYojimbo();
}
void NetworkedClient::on_awake() {
    uint64_t clientId = 0;
    yojimbo::random_bytes((uint8_t*)&clientId, 8);
    log::debug("client id is %.16" PRIx64 "\n", clientId);

    m_client =
        std::make_shared<yojimbo::Client>(yojimbo::GetDefaultAllocator(), Address("0.0.0.0"), config, adapter, time);
}
void NetworkedClient::on_update(double m_delta_time) {
    m_totalTime += m_delta_time;
}
void NetworkedClient::on_fixed_update() {
    Subsystem::on_fixed_update();
}
void NetworkedClient::on_late_update() {
    Subsystem::on_late_update();
}
void NetworkedClient::on_destroy() {
    ShutdownYojimbo();
}
}  // namespace knot
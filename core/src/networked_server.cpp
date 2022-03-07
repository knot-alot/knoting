#include <knoting/engine.h>
#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine), m_server(nullptr) {
    InitializeYojimbo();
    m_config.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
    m_config.timeout = 5;
    m_config.networkSimulator = false;
}
void NetworkedServer::on_awake() {
    uint8_t privateKey[KeyBytes];
    memset(privateKey, 0, KeyBytes);

    m_server = std::make_shared<Server>(GetDefaultAllocator(), privateKey, Address("127.0.0.1", SERVER_PORT), m_config,
                                        clientAdapter, get_time());
    m_server->Start(MAX_CLIENTS);
    log::debug("STARTED SERVER...");
}
void NetworkedServer::on_update(double m_delta_time) {
    if (!m_server->IsRunning()) {
        log::debug("server not running!");
        return;
    }
    if (m_server->GetNumConnectedClients() > 0) {
        m_server->SendPackets();
        m_server->ReceivePackets();
    }
    m_server->AdvanceTime(get_time());
}
void NetworkedServer::on_fixed_update() {}
void NetworkedServer::on_late_update() {}
void NetworkedServer::on_destroy() {
    m_server->DisconnectAllClients();
    m_server->Stop();
    m_server = nullptr;
}
double NetworkedServer::get_time() {
    auto engOpt = Engine::get_active_engine();
    if (engOpt) {
        return engOpt.value().get().get_current_time();
    }
}
}  // namespace knot
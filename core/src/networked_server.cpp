#include <knoting/engine.h>
#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine), m_server(nullptr) {
    InitializeYojimbo();
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
        if (m_server->CanSendMessage(0, 1)) {
            log::debug("Server Attempts to send message");
            ServerMessage* mess = (ServerMessage*)m_server->CreateMessage(0, MessageTypes::SERVER_MESSAGE);
            mess->setSequence(seq);
            seq++;
            m_server->SendMessage(0, 1, mess);
        }
    }
    m_server->SendPackets();
    m_server->ReceivePackets();
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
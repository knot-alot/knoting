#include <knoting/engine.h>
#include <knoting/networked_client.h>

namespace knot {
using namespace yojimbo;
NetworkedClient::~NetworkedClient() {

}

NetworkedClient::NetworkedClient(Engine& engine) : m_engine(engine), m_client(nullptr) {}
void NetworkedClient::on_awake() {
    m_client = std::make_shared<Client>(GetDefaultAllocator(), Address("0.0.0.0", CLIENT_PORT), m_config, clientAdapter,
                                        get_time());

    log::debug("CREATED CLIENT with {} channels", m_config.numChannels);
}
void NetworkedClient::on_update(double m_delta_time) {
    if (m_client->IsDisconnected()) {
        log::debug("Client is not Connected to Server!");
        while (!attempt_connection())
            ;
        return;
    }

    m_client->SendPackets();
    m_client->ReceivePackets();

    if (m_client->IsConnected()) {
        if (!connected) {
            connected = true;
            log::debug("Client {} connected to server", m_clientId);
        }
        handle_recieved_packets();
    }

    m_client->AdvanceTime(get_time());
}

void NetworkedClient::on_fixed_update() {}
void NetworkedClient::on_late_update() {
    Subsystem::on_late_update();
}
void NetworkedClient::on_destroy() {
    m_client->Disconnect();
    m_client = nullptr;
    ShutdownYojimbo();
}
double NetworkedClient::get_time() {
    auto engOpt = Engine::get_active_engine();
    if (engOpt) {
        return engOpt.value().get().get_current_time();
    }
}
bool NetworkedClient::attempt_connection() {
    if (m_client->IsConnecting()) {
        return true;
    }
    yojimbo::random_bytes((uint8_t*)&m_clientId, 8);
    m_serverAddress = Address(SERVER_ADDRESS, SERVER_PORT);

    uint8_t privateNetworkingKey[yojimbo::KeyBytes];
    memset(privateNetworkingKey, 0, KeyBytes);
    m_client->InsecureConnect(privateNetworkingKey, m_clientId, m_serverAddress);
    log::debug("ATTEMPTED CLIENT CONNECTION");
    return m_client->IsConnecting();
}
bool NetworkedClient::handle_recieved_packets() {
    if (!m_client->IsConnected()) {
        return false;
    }
    if (!(m_client)) {
        return false;
    }
    Message* serMess = nullptr;
    while ((serMess = m_client->ReceiveMessage(1))) {
        log::debug("Client received Message {} from Server", serMess->GetType());
        m_client->ReleaseMessage(serMess);
    }
    return true;
}
}  // namespace knot
#include <knoting/engine.h>
#include <knoting/networked_client.h>

namespace knot {
using namespace yojimbo;
NetworkedClient::~NetworkedClient() {
    m_config.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
    m_config.timeout = 5;
    m_config.networkSimulator = false;
}

NetworkedClient::NetworkedClient(Engine& engine) : m_engine(engine), m_client(nullptr) {}
void NetworkedClient::on_awake() {
    m_client = std::make_shared<Client>(GetDefaultAllocator(), Address("0.0.0.0", CLIENT_PORT), m_config, clientAdapter,
                                        get_time());

    log::debug("CREATED CLIENT");
}
void NetworkedClient::on_update(double m_delta_time) {
    if (m_client->IsDisconnected()) {
        log::debug("Client has Disconnected!");
        attempt_connection();
        return;
    } else {
        m_client->SendPackets();
        m_client->ReceivePackets();
    }
    if (m_client->IsConnected()) {
        log::debug("I F***ing Worked");
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
    uint64_t clientId = 0;
    yojimbo::random_bytes((uint8_t*)&clientId, 8);
    m_serverAddress = Address(SERVER_ADDRESS, SERVER_PORT);

    uint8_t privateNetworkingKey[yojimbo::KeyBytes];
    memset(privateNetworkingKey, 0, KeyBytes);
    m_client->InsecureConnect(privateNetworkingKey, m_clientId, m_serverAddress);
    log::debug("ATTEMPTED CLIENT CONNECTION");
    return m_client->IsConnected();
}
}  // namespace knot
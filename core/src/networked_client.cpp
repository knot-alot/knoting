#include <knoting/engine.h>
#include <knoting/networked_client.h>

namespace knot {
using namespace yojimbo;
NetworkedClient::~NetworkedClient() {}

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

    m_tickTime += m_delta_time;
    if (m_client->IsConnected()) {
        send_message();
    }
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
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
    Message* mess = nullptr;
    while ((mess = m_client->ReceiveMessage(1))) {
        if (mess->GetType() == MessageTypes::SERVER_MESSAGE) {
            ServerMessage* serMess = (ServerMessage*)mess;
            serverSeq = serMess->get_sequence();
            serverAck = serMess->get_recent_ack();
            log::debug("Client received Message {} from Server and Server Acknowledged Message: {}", serverSeq,
                       serverAck);
        }
        m_client->ReleaseMessage(mess);
    }
    return true;
}
bool NetworkedClient::send_message() {
    if (m_tickTime >= TICK) {
        if (m_client->CanSendMessage(1)) {
            ClientMessage* mess = (ClientMessage*)m_client->CreateMessage(MessageTypes::CLIENT_MESSAGE);
            mess->set_sequence(seq);
            mess->set_ack(serverSeq);
            seq++;
            m_client->SendMessage(1, mess);
        }
    }
    return true;
}
}  // namespace knot
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
    seq.fill(0);
    cliSeq.fill(0);
    cliAck.fill(0);
}
void NetworkedServer::on_update(double m_delta_time) {
    if (!m_server->IsRunning()) {
        log::debug("server not running!");
        return;
    }
    m_tickTime += m_delta_time;
    if (m_server->GetNumConnectedClients() > 0) {
        send_message();
    }
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }
    m_server->SendPackets();
    m_server->ReceivePackets();

    if (m_server->GetNumConnectedClients() > 0) {
        handle_recieved_packets();
    } else {
        seq.fill(0);
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
bool NetworkedServer::send_message() {
    if (m_tickTime >= TICK) {
        for (int i = 0; i < m_server->GetNumConnectedClients(); ++i) {
            if (m_server->CanSendMessage(i, 1)) {
                // log::debug("Server Attempts to send message");
                ServerMessage* mess = (ServerMessage*)m_server->CreateMessage(0, MessageTypes::SERVER_MESSAGE);
                mess->set_sequence(seq[i]);
                mess->set_ack(cliSeq[i]);
                mess->playerPos.fill(vec3(1, 1, 1));
                mess->playerRots.fill(quat(1, 2, 2, 2));
                for (int16_t& hp : mess->playerHealth) {
                    if (hp > 1) {
                        hp--;
                    } else {
                        hp = 100;
                    }
                }
                seq[i]++;
                m_server->SendMessage(0, 1, mess);
            }
        }
    }
    return true;
}
bool NetworkedServer::handle_recieved_packets() {
    if (!m_server->IsRunning()) {
        return false;
    }
    if (!(m_server)) {
        return false;
    }
    Message* mess = nullptr;
    for (int i = 0; i < m_server->GetNumConnectedClients(); ++i) {
        while ((mess = m_server->ReceiveMessage(i, 1))) {
            if (mess->GetType() == MessageTypes::CLIENT_MESSAGE) {
                ClientMessage* cliMess = (ClientMessage*)mess;
                cliSeq[i] = cliMess->get_sequence();
                cliAck[i] = cliMess->get_recent_ack();
                // log::debug("Server received Message {} from client {}. Client Acknowledged message {}", cliSeq[i], i,
                // cliAck[i]);
                log::debug("### - SERVER - ###");
                log::debug("look Direction: x: {} y: {} ", cliMess->m_lookAxis.x, cliMess->m_lookAxis.y);
                log::debug("move Direction: x: {} y: {} ", cliMess->m_moveAxis.x, cliMess->m_moveAxis.y);
                if (cliMess->jumpPressed)
                    log::debug("Jump Pressed");
                if (cliMess->isShooting)
                    log::debug("PEW PEW!");
                log::debug(" ");
            }
            m_server->ReleaseMessage(i, mess);
        }
    }
    return true;
}
}  // namespace knot
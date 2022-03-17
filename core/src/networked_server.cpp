#include <knoting/engine.h>
#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine), m_server(nullptr) {

}
void NetworkedServer::on_awake() {
    uint8_t privateKey[KeyBytes] = {0};

    m_server = std::make_shared<Server>(GetDefaultAllocator(), privateKey, Address(SERVER_ADDRESS, SERVER_PORT),
                                        m_config, gameAdapter, get_time());
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

    m_server->AdvanceTime(get_time());
    m_server->ReceivePackets();
    if (m_server->GetNumConnectedClients() > 0) {
        handle_recieved_packets();
    } else {
        seq.fill(0);
    }
    m_tickTime += m_delta_time;
    if (m_server->GetNumConnectedClients() > 0) {
        send_message();
    }
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }
    m_server->SendPackets();
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
                Message* mess = generateMessage(i);
                seq[i]++;
                m_server->SendMessage(i, 1, mess);
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
                log::debug("CLIENT {}:", i);
                log::debug("look Direction: x: {} y: {} ", cliMess->m_lookAxis.x, cliMess->m_lookAxis.y);
                log::debug("move Direction: x: {} y: {} ", cliMess->m_moveAxis.x, cliMess->m_moveAxis.y);
                if (cliMess->jumpPressed)
                    log::debug("Jump Pressed");
                if (cliMess->isShooting)
                    log::debug("PEW PEW!");
                log::debug(" ");

                auto sceneOpt = Scene::get_active_scene();
                if (!sceneOpt) {
                    return false;
                }
                Scene& scene = sceneOpt.value();
                entt::registry& registry = scene.get_registry();

                auto players = registry.view<components::ClientPlayer, components::RigidController>();

                for (auto playerHandle : players) {
                    auto playerOpt = scene.get_game_object_from_handle(playerHandle);
                    if (!playerOpt) {
                        continue;
                    }
                    auto playerGO = playerOpt.value();
                    auto& playerComp = playerGO.get_component<components::ClientPlayer>();
                    auto& transform = playerGO.get_component<components::Transform>();
                    auto& rigidController = playerGO.get_component<components::RigidController>();

                    uint16_t playerNum = playerComp.get_client_num();

                    if (!(playerNum == i)) {
                        continue;
                    }

                    playerComp.set_XZ_movement_axis(cliMess->m_moveAxis);
                    playerComp.set_look_axis(cliMess->m_lookAxis);
                    playerComp.set_jumping_pressed(cliMess->jumpPressed);
                    playerComp.set_is_shooting(cliMess->isShooting);

                    vec2i inputs = playerComp.get_movement_axis();
                    vec3 playerInputs = vec3(-inputs.x, 0, -inputs.y);

                    if (glm::abs(playerInputs.x + playerInputs.y + playerInputs.z) <= 0.1f) {
                        continue;
                    }

                    vec3 normInp = glm::normalize(playerInputs);
                    const vec3 directionForce = (0.1f) * 100.0f * 1000.0f * vec3(normInp.x, 0, normInp.z);

                    rigidController.add_force(directionForce);
                }
            }
            m_server->ReleaseMessage(i, mess);
        }
    }
    return true;
}
Message* NetworkedServer::generateMessage(uint16_t cliNum) {
    ServerMessage* mess = (ServerMessage*)m_server->CreateMessage(cliNum, MessageTypes::SERVER_MESSAGE);
    mess->set_sequence(seq[cliNum]);
    mess->set_ack(cliSeq[cliNum]);
    mess->m_clientNum = cliNum;

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return nullptr;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto players = registry.view<components::ClientPlayer>();

    for (auto playerHandle : players) {
        auto playerOpt = scene.get_game_object_from_handle(playerHandle);
        if (!playerOpt) {
            continue;
        }
        auto playerGO = playerOpt.value();

        auto& playerComp = playerGO.get_component<components::ClientPlayer>();
        auto& transform = playerGO.get_component<components::Transform>();

        uint16_t cliNum = playerComp.get_client_num();

        mess->playerPos[cliNum] = transform.get_position();
        mess->playerRots[cliNum] = transform.get_rotation();
        mess->playerHealth[cliNum] = 92;
        mess->paintCollisions.fill(vec3(1, 5, -1));
    }
    return mess;
}
}  // namespace knot
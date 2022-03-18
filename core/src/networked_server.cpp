#include <knoting/engine.h>
#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine), m_server(nullptr) {}

void NetworkedServer::on_awake() {
    std::array<uint8_t, KeyBytes> privateKey = {0};
    char* serverAddress = std::getenv("KNOTING_SERVER");
    std::string serAddress = serverAddress;
    if (!serverAddress) {
        log::info("environment variable KNOTING_SERVER not set");
        serAddress = "127.0.0.1";
    }
    m_server = std::make_shared<Server>(GetDefaultAllocator(), privateKey.data(),
                                        Address(serAddress.c_str(), SERVER_PORT), m_config, gameAdapter, get_time());
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

    m_tickTime += m_delta_time;
    if (m_server->GetNumConnectedClients() > 0) {
        handle_recieved_packets(m_delta_time);
        send_message();
    } else {
        seq.fill(0);
    }
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }
    m_server->SendPackets();
}

void NetworkedServer::on_destroy() {
    m_server->DisconnectAllClients();
    m_server->Stop();
    m_server = nullptr;
}

double NetworkedServer::get_time() {
    auto engOpt = Engine::get_active_engine();
    if (engOpt) {
        return engOpt.value().get().get_window_module().lock()->get_current_time();
    }
}

bool NetworkedServer::send_message() {
    if (m_tickTime >= TICK) {
        for (int i = 0; i < m_server->GetNumConnectedClients(); ++i) {
            if (m_server->CanSendMessage(i, 1)) {
                Message* mess = generateMessage(i);
                seq[i]++;
                m_server->SendMessage(i, 1, mess);
            }
        }
    }
    return true;
}

bool NetworkedServer::handle_recieved_packets(double m_delta_time) {
    if (!m_server->IsRunning()) {
        return false;
    }
    if (!(m_server)) {
        return false;
    }

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return false;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto players = registry.view<components::ClientPlayer, components::RigidController>();

    Message* mess = nullptr;
    for (int i = 0; i < m_server->GetNumConnectedClients(); ++i) {
        while ((mess = m_server->ReceiveMessage(i, 1))) {
            if (mess->GetType() != static_cast<size_t>(NetworkMessageTypes::ClientMessage)) {
                continue;
            }
            ClientMessage* cliMess = (ClientMessage*)mess;
            cliSeq[i] = cliMess->m_sequence;
            cliAck[i] = cliMess->m_recentAck;
            for (auto playerHandle : players) {
                auto playerOpt = scene.get_game_object_from_handle(playerHandle);
                if (!playerOpt) {
                    continue;
                }
                auto playerGO = playerOpt.value();
                auto& playerComp = playerGO.get_component<components::ClientPlayer>();
                auto& transform = playerGO.get_component<components::Transform>();
                auto& rigidController = playerGO.get_component<components::RigidController>();

                uint16_t playerNum = playerComp.m_clientNum;

                if (!(playerNum == i)) {
                    continue;
                }

                playerComp.m_moveAxis = (cliMess->m_moveAxis);
                playerComp.m_lookAxis = (cliMess->m_lookAxis);
                playerComp.m_jumpPressed = (cliMess->jumpPressed);
                playerComp.m_isShooting = (cliMess->isShooting);

                // here downwards will be removed and handled by scripts

                vec2i inputs = playerComp.m_moveAxis;
                vec3 playerInputs = vec3(inputs.x, 0, -inputs.y);

                float boost = 1.0f;
                if (playerComp.m_jumpPressed) {
                    boost = 200.0f;
                }

                if (glm::length(playerInputs) <= 0.1f) {
                    continue;
                }

                vec3 normInp = glm::normalize(playerInputs);
                float baseMulti = 1000000.0f;
                float speedMulti = 0.2f;

                const vec3 directionForce =
                    (float)(m_delta_time * baseMulti * speedMulti * boost) * vec3(normInp.x, 0, normInp.z);

                rigidController.add_force(directionForce);
            }

            m_server->ReleaseMessage(i, mess);
        }
    }
    return true;
}

Message* NetworkedServer::generateMessage(uint16_t cliNum) {
    ServerMessage* mess =
        (ServerMessage*)m_server->CreateMessage(cliNum, static_cast<size_t>(NetworkMessageTypes::ServerMessage));
    mess->m_sequence = (seq[cliNum]);
    mess->m_recentAck = (cliSeq[cliNum]);
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

        uint16_t cliNum = playerComp.m_clientNum;
        mess->playerPos[cliNum] = transform.get_position();
        mess->playerRots[cliNum] = transform.get_rotation();
        mess->playerHealth[cliNum] = 92;
        mess->paintCollisions.fill(vec3(1, 5, -1));
    }
    return mess;
}

}  // namespace knot
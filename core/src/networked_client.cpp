#include <knoting/engine.h>
#include <knoting/networked_client.h>

namespace knot {
using namespace yojimbo;
NetworkedClient::~NetworkedClient() {}

NetworkedClient::NetworkedClient(Engine& engine) : m_engine(engine), m_client(nullptr) {}

void NetworkedClient::on_awake() {
    m_client = std::make_shared<Client>(GetDefaultAllocator(), Address("0.0.0.0", CLIENT_PORT), m_config, gameAdapter,
                                        get_time());
    m_inManager = m_engine.get_window_module().lock()->get_input_manager();
    log::debug("CREATED CLIENT with {} channels", m_config.numChannels);
}
void NetworkedClient::on_update(double m_delta_time) {
    if (m_client->IsDisconnected()) {
        log::debug("Client is not Connected to Server!");
        while (!attempt_connection())
            ;
        return;
    }
    m_client->AdvanceTime(get_time());
    m_client->ReceivePackets();

    if (m_client->IsConnected()) {
        if (!connected) {
            connected = true;
            log::debug("Client {} connected to server", m_clientId);
        }
        handle_recieved_packets();
    }

    m_tickTime += m_delta_time;
    if (m_client->IsConnected()) {
        test_player_input();
        send_message();
    }
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }

    m_client->SendPackets();
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

    uint8_t privateNetworkingKey[yojimbo::KeyBytes] = {0};
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
            if (!cliNumSet) {
                m_clientNum = serMess->m_clientNum;
            }
            log::debug("### - CLIENT {}- ###", m_clientNum);
            log::debug("Player {} position : x: {} y: {} z: {}", serMess->m_clientNum,serMess->playerPos[serMess->m_clientNum].x, serMess->playerPos[serMess->m_clientNum].y,
                       serMess->playerPos[serMess->m_clientNum].z);
            log::debug("Player {} rotation : x: {} y: {} z: {} w: {}",serMess->m_clientNum, serMess->playerRots[serMess->m_clientNum].x,
                       serMess->playerRots[serMess->m_clientNum].y, serMess->playerRots[serMess->m_clientNum].z, serMess->playerRots[serMess->m_clientNum].w);
            log::debug("Player 0 hp: {}", serMess->playerHealth[serMess->m_clientNum]);
            log::debug(" ");

            auto sceneOpt = Scene::get_active_scene();
            if (!sceneOpt) {
                return false;
            }
            Scene& scene = sceneOpt.value();
            entt::registry& registry = scene.get_registry();

            auto players = registry.view<components::ClientPlayer, components::RigidBody>();

            for (auto playerHandle : players) {
                auto playerOpt = scene.get_game_object_from_handle(playerHandle);
                if (!playerOpt) {
                    continue;
                }
                auto playerGO = playerOpt.value();
                auto& playerComp = playerGO.get_component<components::ClientPlayer>();
                auto& transform = playerGO.get_component<components::Transform>();
                auto& rigidBody = playerGO.get_component<components::RigidBody>();

                uint16_t playerNum = playerComp.get_client_num();
                if (!(m_clientNum == playerNum)) {
                    transform.set_position(serMess->playerPos[playerNum]);
                    transform.set_rotation(serMess->playerRots[playerNum]);
                    rigidBody.set_transform(serMess->playerPos[playerNum], serMess->playerRots[playerNum]);
                    // TODO: set player health, pass the paint collisions to somewhere they can be handled?
                }
            }
        }
        m_client->ReleaseMessage(mess);
    }
    return true;
}
bool NetworkedClient::send_message() {
    if (m_tickTime >= TICK) {
        if (m_client->CanSendMessage(1)) {
            Message* mess = generate_message();
            if (mess) {
                m_client->SendMessage(1, mess);
                seq++;
            }
        } else {
            return false;
        }
    }
    return true;
}
Message* NetworkedClient::generate_message() {
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

        if (!(playerComp.get_client_num() == m_clientNum)) {
            continue;
        }
        auto* mess = (ClientMessage*)m_client->CreateMessage(MessageTypes::CLIENT_MESSAGE);
        mess->set_sequence(seq);
        mess->set_ack(serverSeq);
        mess->m_lookAxis = playerComp.get_look_axis();
        mess->m_moveAxis = playerComp.get_movement_axis();
        mess->jumpPressed = playerComp.get_jumping_pressed();
        mess->isShooting = playerComp.get_is_shooting();
        return mess;
    }
    return nullptr;
}
void NetworkedClient::test_player_input() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
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

        if (!(playerComp.get_client_num() == m_clientNum)) {
            continue;
        }

        playerComp.set_is_shooting(!playerComp.get_is_shooting());
        playerComp.set_jumping_pressed(!playerComp.get_jumping_pressed());

        vec3 m_playerInputs = vec3();
        if (m_inManager->key_pressed(KeyCode::A)) {
            m_playerInputs.x += 1;
        }
        if (m_inManager->key_pressed(KeyCode::D)) {
            m_playerInputs.x -= 1;
        }
        if (m_inManager->key_pressed(KeyCode::W)) {
            m_playerInputs.z += 1;
        }
        if (m_inManager->key_pressed(KeyCode::S)) {
            m_playerInputs.z -= 1;
        }

        playerComp.set_XZ_movement_axis(vec2i(m_playerInputs.x, m_playerInputs.z));
        playerComp.set_look_axis(vec2i(0, -1));
    }
}
}  // namespace knot
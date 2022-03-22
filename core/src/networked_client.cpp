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
        while (!attempt_connection()) {
        }
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
        test_player_input();
        send_message();
    }
    reset_tick(m_delta_time);
    m_client->SendPackets();
}

void NetworkedClient::on_destroy() {
    m_client->Disconnect();
    m_client = nullptr;
}

double NetworkedClient::get_time() {
    auto engOpt = Engine::get_active_engine();
    if (!engOpt) {
        return 0.0;
    }
    return engOpt.value().get().get_window_module().lock()->get_current_time();
}

bool NetworkedClient::attempt_connection() {
    if (m_client->IsConnecting()) {
        return true;
    }
    yojimbo::random_bytes((uint8_t*)&m_clientId, 8);
    std::array<uint8_t, yojimbo::KeyBytes> privateNetworkingKey = {0};

    char* serverAddress = std::getenv("KNOTING_SERVER");
    std::string serAddress = "127.0.0.1";
    if (serverAddress) {
        serAddress = serverAddress;
    } else {
        log::info("environment variable KNOTING_SERVER not set");
    }
    m_serverAddress = Address(serAddress.c_str(), SERVER_PORT);

    m_client->InsecureConnect(privateNetworkingKey.data(), m_clientId, m_serverAddress);
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

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return false;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto players = registry.view<components::ClientPlayer, components::RigidBody>();

    Message* mess = nullptr;

    while ((mess = m_client->ReceiveMessage(1))) {
        if (mess->GetType() != static_cast<size_t>(NetworkMessageTypes::ServerMessage)) {
            continue;
        }
        auto serMess = (ServerMessage*)mess;
        serverSeq = serMess->m_sequence;
        serverAck = serMess->m_recentAck;

        for (auto playerHandle : players) {
            auto playerOpt = scene.get_game_object_from_handle(playerHandle);
            if (!playerOpt) {
                continue;
            }
            auto playerGO = playerOpt.value();
            auto& playerComp = playerGO.get_component<components::ClientPlayer>();
            auto& transform = playerGO.get_component<components::Transform>();
            auto& rigidBody = playerGO.get_component<components::RigidBody>();

            if (!cliNumSet) {
                playerComp.m_thisClientNum = serMess->m_clientNum;
                cliNumSet = true;
            }

            uint16_t playerNum = playerComp.m_clientNum;
            transform.set_position(serMess->playerPos[playerNum]);
            transform.set_rotation(serMess->playerRots[playerNum]);
            rigidBody.set_transform(serMess->playerPos[playerNum], serMess->playerRots[playerNum]);
            // TODO: set player health, pass the paint collisions to somewhere they can be handled?
        }

        m_client->ReleaseMessage(mess);
    }
    return true;
}

bool NetworkedClient::send_message() {
    if (m_tickTime < TICK) {
        return false;
    } else {
        if (m_client->CanSendMessage(1)) {
            Message* mess = generate_message();
            if (mess) {
                m_client->SendMessage(1, mess);
                seq++;
            }
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

        if (playerComp.m_clientNum != playerComp.m_thisClientNum) {
            continue;
        }
        auto* mess = (ClientMessage*)m_client->CreateMessage(static_cast<size_t>(NetworkMessageTypes::ClientMessage));
        mess->m_sequence = seq;
        mess->m_recentAck = serverSeq;
        mess->m_lookAxis = playerComp.m_lookAxis;
        mess->m_moveAxis = playerComp.m_moveAxis;
        mess->jumpPressed = playerComp.m_jumpPressed;
        mess->isShooting = playerComp.m_isShooting;
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

        if (playerComp.m_clientNum != playerComp.m_thisClientNum) {
            continue;
        }
        
        vec3 m_playerInputs = vec3();
        if (m_inManager->key_pressed(KeyCode::A)) {
            m_playerInputs.x -= 1;
        }
        if (m_inManager->key_pressed(KeyCode::D)) {
            m_playerInputs.x += 1;
        }
        if (m_inManager->key_pressed(KeyCode::W)) {
            m_playerInputs.z += 1;
        }
        if (m_inManager->key_pressed(KeyCode::S)) {
            m_playerInputs.z -= 1;
        }

        if (m_inManager->key_on_trigger(KeyCode::Space)) {
            playerComp.m_jumpPressed = true;
        } else {
            playerComp.m_jumpPressed = false;
        }

        playerComp.m_moveAxis = (vec2i(m_playerInputs.x, m_playerInputs.z));
        playerComp.m_lookAxis = (vec2i(0, -1));
    }
}
void NetworkedClient::reset_tick(double m_delta_time) {
    m_tickTime += m_delta_time;
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }
}

}  // namespace knot
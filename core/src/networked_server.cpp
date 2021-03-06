#include <knoting/engine.h>
#include <knoting/networked_server.h>

namespace knot {
NetworkedServer::~NetworkedServer() {}

NetworkedServer::NetworkedServer(Engine& engine) : m_engine(engine), m_server(nullptr) {}

void NetworkedServer::on_awake() {
    std::array<uint8_t, KeyBytes> privateKey = {0};
    char* serverAddress = std::getenv("KNOTING_SERVER");
    std::string serAddress = "127.0.0.1";
    if (serverAddress) {
        serAddress = serverAddress;
    } else {
        log::info("environment variable KNOTING_SERVER not set");
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
    update_collisions_buffer();
    if (!m_server->IsRunning()) {
        log::debug("server not running!");
        return;
    }
    m_server->AdvanceTime(get_time());
    m_server->ReceivePackets();

    if (m_server->GetNumConnectedClients() > 0) {
        handle_recieved_packets(m_delta_time);
        send_message();
    } else {
        seq.fill(0);
    }
    reset_tick(m_delta_time);
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

                if (playerNum != i) {
                    continue;
                }

                playerComp.m_moveAxis = (cliMess->m_moveAxis);
                playerComp.m_lookAxis = (cliMess->m_lookAxis);
                playerComp.m_jumpPressed = (cliMess->jumpPressed);
                playerComp.m_isShooting = (cliMess->isShooting);

                //                log::debug("player #{} is moving in axis {}", playerNum,
                //                to_string(playerComp.m_moveAxis));

                //                // here downwards will be removed and handled by scripts
                //
                //                vec2i inputs = playerComp.m_moveAxis;
                //                vec3 playerInputs = vec3(inputs.x, 0, -inputs.y);
                //
                //                float boost = 1.0f;
                //                if (playerComp.m_jumpPressed) {
                //                    boost = 200.0f;
                //                }
                //
                //                if (glm::length(playerInputs) <= 0.1f) {
                //                    continue;
                //                }
                //
                //                vec3 normInp = glm::normalize(playerInputs);
                //                float baseMulti = 1000000.0f;
                //                float speedMulti = 0.2f;
                //
                //                const vec3 directionForce =
                //                    (float)(m_delta_time * baseMulti * speedMulti * boost) * vec3(normInp.x, 0,
                //                    normInp.z);
                //
                //                rigidController.add_force(directionForce);
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
        mess->paintCollisions.fill(vec4(0));

        int paintCollisionBufferSize = paintCollisionBuffer.size();
        for (int i = 0; i < 20; i++) {
            if (paintCollisionBufferSize < 1)
                break;
            mess->paintCollisions[i] = paintCollisionBuffer.front();
            paintCollisionBuffer.pop_front();
            paintCollisionBufferSize--;
        }
        paintCollisionBuffer.clear();
    }
    return mess;
}
void NetworkedServer::reset_tick(double m_delta_time) {
    m_tickTime += m_delta_time;
    if (m_tickTime >= TICK) {
        m_tickTime -= TICK;
    }
}
void NetworkedServer::update_collisions_buffer() {
    using namespace components;

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto collView = registry.view<Collision_Detection>();
    std::vector<actor_contact_data> contacts;
    for (auto entity : collView) {
        auto goOpt = scene.get_game_object_from_handle(entity);
        if (!goOpt) {
            continue;
        }
        auto go = goOpt.value();

        Collision_Detection& colDetect = go.get_component<Collision_Detection>();
        contacts = colDetect.get_actor_contact_data();
        break;
    }

    for (auto& contact : contacts) {
        bool isDynamic = contact.m_search_actor.m_is_dynamic;
        std::string contact_name = "";
        if (isDynamic) {
            auto dynamActor = contact.m_search_actor.m_search_dynamic;
            if (dynamActor->get()->getName()) {
                contact_name = dynamActor->get()->getName();
            }
        } else {
            auto staticActor = contact.m_search_actor.m_search_static;
            if (staticActor->get()->getName()) {
                contact_name = staticActor->get()->getName();
            }
        }
        if (!(contact_name == "RED" || contact_name == "BLUE")) {
            continue;
        }
        auto& bulletContacts = contact.m_contact_data;
        for (auto& bulletContact : bulletContacts) {
            if (!(bulletContact.m_contact_actor->getType() == PxActorType::eRIGID_STATIC)) {
                continue;
            }
            int colour = 0;
            if (contact_name == "RED") {
                colour = static_cast<size_t>(Team::RED);
            } else {
                colour = static_cast<size_t>(Team::BLUE);
            }
            vec4 collisionPoint = vec4(bulletContact.m_contact_point, colour);
            paintCollisionBuffer.push_back(collisionPoint);
        }
    }
}

}  // namespace knot
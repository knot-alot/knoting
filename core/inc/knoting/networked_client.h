#pragma once

#include <inttypes.h>
#include <knoting/log.h>
#include <knoting/networking_types.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <yojimbo.h>

namespace knot {

class Engine;

}

// namespace knot {
// using namespace yojimbo;
//
// constexpr int SERVER_PORT = 42069;
// constexpr int CLIENT_PORT = 42068;
// const char* const SERVER_ADDRESS = "127.0.0.1";
//
// class ClientMessage : public Message {
//    public:
//     ClientMessage() : sequence(0) {}
//
//     template <typename Stream>
//     bool Serialize(Stream& stream) {
//         serialize_bits(stream, sequence, 16);
//         return true;
//     }
//
//     YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
//
//    protected:
//     uint16_t sequence;
// };
//
// YOJIMBO_MESSAGE_FACTORY_START(ClientMessageFactory, 1);
// YOJIMBO_DECLARE_MESSAGE_TYPE(0, ClientMessage);
// YOJIMBO_MESSAGE_FACTORY_FINISH();
//
// class ClientAdapter : public Adapter {
//    public:
//     MessageFactory* CreateMessageFactory(Allocator& allocator) {
//         return YOJIMBO_NEW(allocator, ClientMessageFactory, allocator);
//     }
// };
// static ClientAdapter clientAdapter;
// }  // namespace knot

namespace knot {
class NetworkedClient : public Subsystem {
   public:
    NetworkedClient(Engine& engine);
    ~NetworkedClient();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_fixed_update() override;
    void on_late_update() override;
    void on_destroy() override;

    double get_time();

    bool attempt_connection();

   protected:
    Engine& m_engine;
    std::shared_ptr<yojimbo::Client> m_client;

    yojimbo::ClientServerConfig m_config;
    uint64_t m_clientId = 0;
    yojimbo::Address m_serverAddress;
    float retryStep = 0.0f;
    const float retryStepMax = 2.0f;
};
}  // namespace knot

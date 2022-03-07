#pragma once

#include <inttypes.h>
#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <yojimbo.h>

namespace knot {

class Engine;

}

namespace knot {
using namespace yojimbo;
class ClientMessage : public Message {};

class ClientAdapter : public Adapter {
   public:
    MessageFactory* CreateMessageFactory(Allocator& allocator) {
        return YOJIMBO_NEW(allocator, TestMessageFactory, allocator);
    }
};
}  // namespace knot
namespace knot {
constexpr int SERVER_PORT = 42069;
const std::string SERVER_ADDRESS = "127.0.0.1";
class NetworkedClient : public Subsystem {
   public:
    NetworkedClient(Engine& engine);
    ~NetworkedClient();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_fixed_update() override;
    void on_late_update() override;
    void on_destroy() override;

   protected:
    Engine& m_engine;
    yojimbo::ClientServerConfig m_config;
    float m_totalTime = 0.0f;
    std::shared_ptr<yojimbo::Client> m_client;
};
}  // namespace knot

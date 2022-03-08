#pragma once

#include <knoting/log.h>
#include <knoting/networking_types.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {

class Engine;

}
namespace knot {

class NetworkedServer : public Subsystem {
   public:
    NetworkedServer(Engine& engine);
    ~NetworkedServer();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_fixed_update() override;
    void on_late_update() override;
    void on_destroy() override;

    double get_time();
    bool handle_recieved_packets();

   protected:
    Engine& m_engine;
    std::shared_ptr<yojimbo::Server> m_server;
    m_clientServerConfig m_config;
    uint16_t seq = 0;
};
}  // namespace knot

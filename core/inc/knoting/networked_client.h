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
class InputManager;

}  // namespace knot
namespace knot {
class NetworkedClient : public Subsystem {
   public:
    NetworkedClient(Engine& engine);
    ~NetworkedClient();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_destroy() override;

    double get_time();

    bool attempt_connection();
    bool handle_recieved_packets();
    bool send_message();
    Message* generate_message();
    void reset_tick(double m_delta_time);

    void test_player_input();

    uint16_t get_client_num() {return m_clientNum;}

   protected:
    Engine& m_engine;
    std::shared_ptr<yojimbo::Client> m_client;
    yojimbo::Address m_serverAddress;
    GameConfig m_config;

    uint16_t seq = 0;
    uint16_t serverSeq = 0;
    uint16_t serverAck = 0;

    uint64_t m_clientId = 0;
    uint16_t m_clientNum = 10;
    bool cliNumSet = false;
    bool connected = false;

    double m_tickTime = 0.0;
    std::shared_ptr<InputManager> m_inManager;
};
}  // namespace knot

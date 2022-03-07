#pragma once

#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {

class Engine;

}
namespace knot {
constexpr int MAX_CLIENTS = 6;

class NetworkedServer : public Subsystem {
   public:
    NetworkedServer(Engine& engine);
    ~NetworkedServer();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_fixed_update() override;
    void on_late_update() override;
    void on_destroy() override;

   protected:
    Engine& m_engine;
    const int m_clientPort = 42068;
    int[] clients;
};
}  // namespace knot

#pragma once

#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {

class Engine;

}
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

   protected:
    Engine& m_engine;
};
}  // namespace knot

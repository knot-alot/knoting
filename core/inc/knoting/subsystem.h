#pragma once

namespace knot {
class Subsystem {
   public:
    Subsystem() {}
    ~Subsystem() {}

    virtual void on_awake() {}

    virtual void on_update(double m_delta_time) {}
    virtual void on_fixedupdate() {}
    virtual void on_late_update() {}

    virtual void on_destroy() {}
};
}  // namespace knot
